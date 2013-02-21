#include "sw.h"

#define MAX_VPORTS    256

static int vport[MAX_VPORTS];
static struct vswitch_instance *mgmt;

static int init_mgmt_port(void)
{
	mgmt = create_vswitch_instance(get_vswitch_name());
	if (!mgmt) {
		printf("%s: error initializing management port, aborting.\n", __func__);
		return -1;
	}
	return 0;
}

static void dump_packet(struct sockaddr_un *rcvsk, char *buf, int len)
{
	int i, j;

	printf("Message from %s:", rcvsk->sun_path);
	for (i = j = 0; i < len; i++) {
		if ((i % 15) == 0) printf("\n\t");
		printf("%02x ", buf[i]);

		if ((i + 1) % 15 == 0 || i == len-1) {
			if (i == len-1) {
				int k;
				for (k = 0; k < 15-i+j; k++) printf("   ");
			}
			printf(": ");
			for (; j <= i; j++) {
				printf("%c", buf[j]);
			}
		}
	}
	printf("\n\n");
}

static void process_packet(struct sockaddr_un *rcvsk, char *buf, int len)
{
	dump_packet(rcvsk, buf, len);
}

#define MAX_PACKET_LEN       1024

static int listen_on_mgmt_port(void)
{
	ssize_t len;
	socklen_t sklen;
	char buf[MAX_PACKET_LEN];
	struct sockaddr_un rcvsk;

	if (!mgmt) return -1;

	while (1) {
		len = recvfrom(mgmt->mgmtfd, buf, MAX_PACKET_LEN, 0,
			       (struct sockaddr *)&rcvsk, &sklen);
		if (len <= 0) {
			if (!len) break;
			perror("recv");
		}
		process_packet(&rcvsk, buf, len);
	}

	return 0;
}

static int close_mgmt_port(void)
{
	delete_vswitch_instance(mgmt);
	return 0;
}

int main(int argc, char *argv[])
{
	if (init_mgmt_port())
		return -1;

	listen_on_mgmt_port();
	close_mgmt_port();

	return 0;
}
