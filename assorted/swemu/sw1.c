#include "sw.h"

static struct switchdev switches[] = {
	{ "emu-sw1", NULL },
	{ "emu-sw2", NULL },
	{ "emu-sw3", NULL },
};

#define ARRAY_SIZE(x)        sizeof(x)/sizeof(x[0])

static inline void ping_vswitch_forever(struct switchdev *dev)
{
	if (!dev) return;

	while (1) {
		sleep(1);
		ping_vswitch(dev->mgmt);
	}
}

static void send_ping_forever(void)
{
	int nrdevs = ARRAY_SIZE(switches), i;
	int pid;

	for (i = 0; i < nrdevs; i++) {
		pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) {
			ping_vswitch_forever(&switches[i]);
		}
	}

	wait(NULL);
}

static int create_switch_devices(void)
{
	int nrdevs = ARRAY_SIZE(switches), i;

	for (i = 0; i < nrdevs; i++) {
		switches[i].mgmt = create_vswitch_instance(switches[i].name);
		if (!switches[i].mgmt) {
			printf("%s: error creating vswitch "
			       "instance for '%s', aborting.\n", __func__,
			       switches[i].name);
			return i;
		}
		connect_vswitch(switches[i].mgmt);
	}

	return i;
}

static void delete_switch_devices(void)
{
	int nrdevs = ARRAY_SIZE(switches), i;

	for (i = 0; i < nrdevs; i++) {
		if (switches[i].mgmt) {
			delete_vswitch_instance(switches[i].mgmt);
			switches[i].mgmt = NULL;
		}
	}
}

int main(int argc, char *argv[])
{
	create_switch_devices();
	send_ping_forever();
	delete_switch_devices();

	return 0;
}
