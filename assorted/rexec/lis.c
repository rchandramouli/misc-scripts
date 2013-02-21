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

#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#define __USE_GNU
#include <pthread.h>

#include "ll.h"

#define SK_LIS_PORT           8128
#define SK_SPK_PORT           5757

/* Receive queue */
struct packet {
	struct list list;
	int len;
	char payload[0];
};
struct rxQ {
	struct list list;
	struct list pkt_head;
	struct in_addr in;
};

struct sock_props {
	int sockfd;
	int port;
	int sock_type;

	struct list rxqhead;

	int (*init)(struct sock_props *);
	int (*action)(struct sock_props *);
	int (*end)(struct sock_props *);
};

static int sk_end(struct sock_props *sk)
{
	if (sk->sockfd > 0)
		close(sk->sockfd);

	/* Free all in the rxqhead */

	return 0;
}

static int sk_init(struct sock_props *sk)
{
	struct sockaddr_in skaddr;
	int reuse = 1;

	sk->sockfd = socket(AF_INET, sk->sock_type, 0);
	if (sk->sockfd < 0) {
		perror("socket");
		return -1;
	}

	/* reuse port address */
	if (setsockopt(sk->sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		perror("setsockopt");
	}

	memset(&skaddr, 0, sizeof(skaddr));
	skaddr.sin_family = AF_INET;
	skaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	skaddr.sin_port = sk->port;

	if (bind(sk->sockfd, (struct sockaddr *)&skaddr, sizeof(skaddr)) < 0) {
		perror("bind");
		if (sk->end) {
			(*sk->end)(sk);
		}
		return -1;
	}

	/* Initialize packet queue head */
	list_init(&sk->rxqhead);

	return 0;
}

static struct rxQ *find_in_rxq(struct list *rxqh, struct in_addr in)
{
	struct list *l;
	struct rxQ *rxq;

	for_each_node_in_list(rxqh, l) {
		rxq = container_of(l, struct rxQ, list);
		if (rxq->in.s_addr == in.s_addr)
			return rxq;
	}
	return NULL;
}

/* Process received packets */
static int process_rx(struct sock_props *s, struct sockaddr_in *skaddr, char *buf, int len)
{
	struct in_addr in = skaddr->sin_addr;
	struct rxQ *rxq;
	struct packet *pkt;

	rxq = find_in_rxq(&s->rxqhead, in);
	if (!rxq) {
		rxq = malloc(sizeof(struct rxQ));
		if (!rxq)
			goto drop_packet;
		rxq->in = in;
		list_init(&rxq->pkt_head);
		list_init(&rxq->list);
		list_add_link(head, &rxq->list);
	}

	pkt = malloc(sizeof(struct packet) + len);
	if (!pkt)
		goto drop_packet;

	pkt->len = len;
	memcpy(pkt->pyload, buf, len);
	list_init(&pkt->list);

	/* Add to list */
	list_add_link(&rxq->pkt_head, &pkt->list);
	return 0;

drop_packet:
	printf("no memory, dropping packet from %s len: %d\n",
	       inet_addr(in), len);
	return -1;
}

/* Keep listening for messages */
#define LIS_BUF_SZ         1024
static int lis_action(struct sock_props *s)
{
	struct sockaddr_in skaddr;
	socklen_t sklen = sizeof(skaddr);
	char buf[LIS_BUF_SZ];
	int len;

	while (1) {
		if ((len = recvfrom(s->sockfd, buf, LIS_BUF_SZ, 0,
				    (struct sockaddr *)&skaddr, &sklen)) < 0) {
			perror("recvfrom");
			continue;
		}
		if (len == 0)
			break;
		buf[len] = 0;
		process_rx(s, &skaddr, buf, len);
	}

	return 0;
}

/* Keep sending messages */
#define SPK_BUF_SZ         1024
static int spk_action(struct sock_props *s)
{
	struct sockaddr_in skaddr;
	socklen_t sklen = sizeof(skaddr);
	char buf[SPK_BUF_SZ], *fmt = "HELLO 0x%08x";
	int len;

	srandom(time(NULL));

	while (1) {
		/* receiver's address */
		skaddr.sin_family = AF_INET;
		//skaddr.sin_addr.s_addr = 0xffffffff; /* broadcast */
		skaddr.sin_addr.s_addr = inet_addr("10.120.89.77");
		skaddr.sin_port = SK_LIS_PORT;

		len = sprintf(buf, fmt, random());
		if ((len = sendto(s->sockfd, buf, len, 0,
				  (struct sockaddr *)&skaddr, sklen)) < 0) {
			perror("sendto");
		}

		/* do so, every 1 second */
		sleep(1);
	}

	return 0;
}

static struct sock_props lis = {
	.port = SK_LIS_PORT,
	.sock_type = AF_INET,
	.init = sk_init,
	.action = lis_action,
	.end = sk_end,
};
static struct sock_props spk = {
	.port = SK_SPK_PORT,
	.sock_type = AF_INET,
	.init = sk_init,
	.action = spk_action,
	.end = sk_end,
};

static void actor_start(struct sock_props *a)
{
	if (!a->init || (*a->init)(a) < 0)
		return;

	if (a->action)
		(*a->action)(a);

	if (a->end)
		(*a->end)(a);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return -1;

	if (argv[1][0] == '-' && argv[1][1] == 'l') {
		actor_start(&lis);
	} else if (argv[1][0] == '-' && argv[1][1] == 's') {
		actor_start(&spk);
	}

	return 0;
}
