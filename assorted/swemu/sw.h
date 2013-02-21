#ifndef SW_H
#define SW_H

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
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

#define MAX_NAME_LEN            108

#define VSOCKET(x)         struct sockaddr_un x

struct list {
	struct list *next, *prev;
};

struct vswitch_instance {
	VSOCKET(sock);
	VSOCKET(vswitch);
	int mgmtfd;
	char flags;
};

struct switchport {
	struct list mirror_list;
	struct list egress_list;
	int portid;
	int portfd;
	struct switchports *ingress;
	VSOCKET(port);
};

struct portlist {
	struct list list;
	struct switchport *port;
};

struct switchdev {
	char *name;
	struct vswitch_instance *mgmt;

	/* Topology */
	char nr_ports;
	struct list port_list;
};

extern const char *get_vswitch_name(void);
extern struct vswitch_instance *create_vswitch_instance(const char *instname);
extern int delete_vswitch_instance(struct vswitch_instance *instance);
extern void connect_vswitch(struct vswitch_instance *instance);
extern int ping_vswitch(struct vswitch_instance *instance);

#endif /* SW_H */
