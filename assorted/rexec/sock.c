#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>
#include <error.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include "ll.h"

#define APP_PROTOCOL            221

#define MSG_TYPE_HEARTBEAT      0x1
#define MSG_TYPE_ACK            0x2

#define MAX_PACKETS             1024
#define MAX_SIBLINGS            512

#define MSG_ERR_NOERR           0
#define MSG_ERR_QUEUE_FULL     -1
#define MSG_ERR_QUEUE_EMPTY    -2
#define MSG_ERR_PDU_INVALID    -3

/*
 * Packet types/Bytes:
 * child-id -> 4 bytes
 *
 * DISCOVER:        0xFF <child-id to> <child-id from>
 * DISCOVER REPLY:  0x7E <child-id to> <child-id from>
 * ALIVE MSG:       0x18 <child-id to> <child-id from>
 * ALIVE REPLY:     0x81 <child-id to>
 */
struct msg_packet {
	unsigned char type;
	pid_t dest;
	pid_t src;
} __attribute__((packed));

#define MSG_PDU_DISCOVER           0xFF
#define MSG_PDU_DISCOVER_ACK       0x7E
#define MSG_PDU_ALIVE              0x18
#define MSG_PDU_ALIVE_ACK          0x81

struct sibqueue {
	int sibs[MAX_SIBLINGS];
	int head, tail, flag, size;
};

struct mqueue {
	struct msg_packet packets[MAX_PACKETS];
	int head, tail, flag, size;
};

struct child {
	pid_t pid;
	int sockfd;

	/* Tx/Rx queues */
	struct mqueue txQ;
	struct mqueue rxQ;

	/* Sibling PQ */
	struct sibqueue sibPQ;
};

/* Message queue handling */
static void mq_queue_init(struct mqueue *q, int size)
{
	q->size = size;
	q->head = q->tail = q->flag = 0;
}

static int mq_enqueue_msg(struct mqueue *q, struct msg_packet *msg)
{
	if (q->head == q->tail && q->flag)
		return MSG_ERR_QUEUE_FULL;

	q->packets[q->tail] = *msg;
	q->tail = (q->tail + 1) % q->size;

	if (q->tail == q->head)
		q->flag = 1;

	return 0;
}

static int mq_dequeue_msg(struct mqueue *q, struct msg_packet *msg)
{
	if (q->head == q->tail && !q->flag)
		return MSG_ERR_QUEUE_EMPTY;

	*msg = q->packets[q->head];
	q->head = (q->head + 1) % q->size;

	if (q->head == q->tail)
		q->flag = 0;

	return 0;
}

/* Sibling Priority Queue handling */
static void sibq_queue_init(struct sibqueue *q, int size)
{
	q->size = size;
	q->head = q->tail = q->flag = 0;
}

static int sibq_enqueue_msg(struct sibqueue *q, int *sib)
{
	if (q->head == q->tail && q->flag)
		return MSG_ERR_QUEUE_FULL;

	q->sibs[q->tail] = *sib;
	q->tail = (q->tail + 1) % q->size;

	if (q->tail == q->head)
		q->flag = 1;

	return 0;
}

static int sibq_dequeue_msg(struct sibqueue *q, int *sib)
{
	if (q->head == q->tail && !q->flag)
		return MSG_ERR_QUEUE_EMPTY;

	*sib = q->sibs[q->head];
	q->head = (q->head + 1) % q->size;

	if (q->head == q->tail)
		q->flag = 0;

	return 0;
}

/* dequeue and then enqueue: suited for round-robin service of
 * siblings.
 */
static int sibq_rotate_msg(struct sibqueue *q, int *sib)
{
	int res;

	if ((res = sibq_dequeue_msg(q, sib)) < 0)
		return res;

	return sibq_enqueue_msg(q, sib);
}

static int sibq_find_key(struct sibqueue *q, int sib)
{
	int i;

	if (q->head == q->tail && !q->flag)
		return 0;

	i = q->head;
	do {
		if (q->sibs[i] == sib)
			return 1;
		i = (i + 1) % q->size;
	} while (i != q->tail);

	return 0;
}

/* Craft a message and then send it! */
#define MAX_BUF_SIZE        64

/* queueing of packets */
static int make_and_queue_packet(struct mqueue *q, char type, int dest, int src)
{
	struct msg_packet msg;

	msg.type = type;
	msg.dest = dest;
	msg.src = src;

	return mq_enqueue_msg(q, &msg);
}

#define queue_alive_packet(q, dest, src)                                 \
              make_and_queue_packet(q, MSG_PDU_ALIVE, dest, src)

#define queue_alive_ack_packet(q, dest, src)                             \
              make_and_queue_packet(q, MSG_PDU_ALIVE_ACK, dest, src)

#define queue_discover_packet(q, src)                                    \
              make_and_queue_packet(q, MSG_PDU_DISCOVER, 0, src)

#define queue_discover_ack_packet(q, dest, src)                          \
              make_and_queue_packet(q, MSG_PDU_DISCOVER_ACK, dest, src)


static void dump_packet(char *prefix, struct child *child, struct msg_packet *msg)
{
	switch (msg->type) {
	case MSG_PDU_DISCOVER:
		printf("-- %s -- C %u: packet (DISCOVER, s: %u)\n",
		       prefix, child->pid, msg->src);
		break;

	case MSG_PDU_DISCOVER_ACK:
		printf("-- %s -- C %u: packet (DISCOVER_ACK, d: %u, s: %u)\n",
		       prefix, child->pid, msg->dest, msg->src);
		break;

	case MSG_PDU_ALIVE:
		printf("-- %s -- C %u: packet (ALIVE, d: %u, s: %u)\n",
		       prefix, child->pid, msg->dest, msg->src);
		break;
		
	case MSG_PDU_ALIVE_ACK:
		printf("-- %s -- C %u: packet (ALIVE_ACK, d: %u, s: %u)\n",
		       prefix, child->pid, msg->dest, child->pid);
		break;

	default:
		printf("-- %s -- C %u: packet (INVALID)\n",
		       prefix, child->pid);
	}
}

static int send_packet(struct child *child, struct msg_packet *msg)
{
	int len, res, dest = msg->dest;
	struct sockaddr_nl nladdr;
	struct msghdr mh;
	struct iovec vec;
	struct {
		struct nlmsghdr header;
		char buf[sizeof(struct msg_packet)];
	} nlpacket;

	/* dump_packet("send", child, msg); */

	len = sizeof(struct msg_packet);
	switch (msg->type) {
	case MSG_PDU_DISCOVER:
		dest = child->pid + 1;
	case MSG_PDU_DISCOVER_ACK:
	case MSG_PDU_ALIVE:
	case MSG_PDU_ALIVE_ACK:
		break;
		
	default:
		return MSG_ERR_PDU_INVALID;
	}

	memset(&nlpacket, 0, sizeof(nlpacket));
	nlpacket.header.nlmsg_len = len + sizeof(nlpacket.header);
	nlpacket.header.nlmsg_type = 0;
	nlpacket.header.nlmsg_flags = NLM_F_REQUEST;
	nlpacket.header.nlmsg_seq = time(NULL);
	nlpacket.header.nlmsg_pid = child->pid;
	memcpy(nlpacket.buf, msg, len);
	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;
	nladdr.nl_pid = dest; /* FIXME */
	nladdr.nl_groups = 0;
	vec.iov_base = &nlpacket;
	vec.iov_len = nlpacket.header.nlmsg_len;
	mh.msg_name = &nladdr;
	mh.msg_namelen = sizeof(nladdr);
	mh.msg_iov = &vec;
	mh.msg_iovlen = 1;
	mh.msg_control = NULL;
	mh.msg_controllen = 0;
	mh.msg_flags = 0;

	res = sendmsg(child->sockfd, &mh, 0);
	if (res < 0) {
		if (errno != ECONNREFUSED)
			perror("sendmsg");
	}

	return res;
}

static int receive_packet(struct child *child, struct msg_packet *msg)
{
	int res;
	struct sockaddr_nl nladdr;
	struct msghdr mh;
	struct iovec vec;
	struct {
		struct nlmsghdr header;
		char buf[sizeof(struct msg_packet)];
	} nlpacket;

	memset(&nlpacket, 0, sizeof(nlpacket));
	nlpacket.header.nlmsg_len = sizeof(struct msg_packet) +
		sizeof(nlpacket.header);
	nlpacket.header.nlmsg_pid = child->pid;
	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;
	vec.iov_base = &nlpacket;
	vec.iov_len = nlpacket.header.nlmsg_len;
	mh.msg_name = &nladdr;
	mh.msg_namelen = sizeof(nladdr);
	mh.msg_iov = &vec;
	mh.msg_iovlen = 1;
	mh.msg_control = NULL;
	mh.msg_controllen = 0;
	mh.msg_flags = 0;

	res = recvmsg(child->sockfd, &mh, MSG_DONTWAIT);
	if (res < 0) {
		if (errno != EAGAIN)
			perror("recvmsg");
		return res;
	}

	memcpy(msg, nlpacket.buf, sizeof(struct msg_packet));
	/* dump_packet("receive", child, msg); */

	return res;
}

static int child_tx(struct child *child)
{
	int res, sib;
	struct msg_packet msg;

	/* Try to fetch a packet from the queue and send it, if there
	 * is no packet in the queue, then, queue up a 'alive' packet
	 * for the first sibling in the queue.
	 */
	res = mq_dequeue_msg(&child->txQ, &msg);
	if (!res)
		return send_packet(child, &msg);

	/* Try sending a ALIVE packet to children */
	res = sibq_rotate_msg(&child->sibPQ, &sib);
	if (!res)
		return queue_alive_packet(&child->txQ, sib, child->pid);

	/* Try sending a DISCOVERY packet */
	return queue_discover_packet(&child->txQ, child->pid);
}

static int process_packet(struct child *child, struct msg_packet *msg)
{
	dump_packet("receive", child, msg);

	switch (msg->type) {
	case MSG_PDU_DISCOVER:
		if (!sibq_find_key(&child->sibPQ, msg->src))
			sibq_enqueue_msg(&child->sibPQ, &msg->src);
		queue_discover_ack_packet(&child->txQ, msg->src, child->pid);
		break;

	case MSG_PDU_DISCOVER_ACK:
		if (!sibq_find_key(&child->sibPQ, msg->src))
			sibq_enqueue_msg(&child->sibPQ, &msg->src);

		break;

	case MSG_PDU_ALIVE:
		queue_alive_ack_packet(&child->txQ, msg->src, child->pid);
		break;
		
	case MSG_PDU_ALIVE_ACK:
		break;

	default:
		return MSG_ERR_PDU_INVALID;
	}

	return 0;
}

static int child_rx(struct child *child)
{
	int res, process = 0;
	struct msg_packet msg, msg_inq;

	/* Dequeue a packet from the receive queue */
	res = mq_dequeue_msg(&child->rxQ, &msg_inq);
	if (!res)
		process = 1;

	/* Receive a packet from the network */
	res = receive_packet(child, &msg);
	if (res > 0) {
		/* Queue the received packet */
		res = mq_enqueue_msg(&child->rxQ, &msg);
		if (res) {
			printf("C %u: packet dropped, type = %02x\n",
			       child->pid, msg.type);
		}
	}

	/* Process the dequeued packet */
	if (!process)
		return 0;

	return process_packet(child, &msg_inq);
}

static int socket_init(struct child *child)
{
	int fd, len;
	struct sockaddr_nl nladdr;

	fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
	if (fd < 0) {
		perror("socket");
		return -1;
	}

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;
	nladdr.nl_pid = child->pid;
	nladdr.nl_groups = 0;

	if (bind(fd, (struct sockaddr *)&nladdr, sizeof(nladdr)) < 0) {
		perror("bind");
		return -1;
	}

	child->sockfd = fd;
	
	return 0;
}

static void child_process(void)
{
	struct child *child;

	/* Init */
	child = malloc(sizeof(struct child));
	if (!child) {
		perror("malloc");
		return;
	}
	child->pid = getpid();

	/* Initialize child queues */
	mq_queue_init(&child->txQ, MAX_PACKETS);
	mq_queue_init(&child->rxQ, MAX_PACKETS);

	sibq_queue_init(&child->sibPQ, MAX_SIBLINGS);

	/* Create socket for communication */
	if (socket_init(child) < 0)
		return;

	/* Round-robin tx/rx activity */
	while (1) {
		child_tx(child);
		sleep(1);

		child_rx(child);
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	int pid, i;

	for (i = 0; i < 16; i++) {
		if ((pid = fork()) < 0) {
			perror("fork");
			continue;
		}

		if (!pid) { /* In child */
			child_process();
			exit(0);
		}
	}

	wait(NULL);

	return 0;
}
