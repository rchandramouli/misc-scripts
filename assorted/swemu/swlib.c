#include "sw.h"
#include "tlv.h"

/* Virtual switch name */
#define VSWITCH_SOCK_DIR     "/tmp/vswitch"
#define VSWITCH_CONNECT      "vswconnect"
#define VSWITCH_MGMT         "mgmt"

static const char *vswitch = "vswconnect";

const char *get_vswitch_name(void)
{
	return vswitch;
}

static int make_directory(const char *path)
{
	struct stat s;

	if (stat(path, &s) < 0) {
		if (errno != ENOENT) return -1;
		if (mkdir(path, S_IRWXU) < 0) {
			perror("mkdir");
			return -1;
		}
	} else if (!S_ISDIR(s.st_mode)) {
		return -1;
	}
	return 0;
}

static int fam_unix_create_sock_path(const char *swname)
{
	char sockpath[MAX_NAME_LEN];

	if (make_directory(VSWITCH_SOCK_DIR))
		return -1;
	sprintf(sockpath, VSWITCH_SOCK_DIR "/%s", swname);
	if (make_directory(sockpath)) {
		perror("mkdir");
		printf("%s: for directory: %s\n", __func__, sockpath);
		return -1;
	}
	return 0;
}

/* pass 0 for management port */
static void fam_unix_get_sock_path(const char *swname, int port, char *buf)
{
	char portstr[8];

	if (port) sprintf(portstr, "port%d", port);
	sprintf(buf, VSWITCH_SOCK_DIR "/%s/%s", swname,
		(!port) ? VSWITCH_MGMT: portstr);
}

void connect_vswitch(struct vswitch_instance *instance)
{
	char vswpath[MAX_NAME_LEN];

	fam_unix_get_sock_path(get_vswitch_name(), 0, vswpath);
	memset(&instance->vswitch, 0, sizeof(struct sockaddr_un));
	instance->vswitch.sun_family = PF_LOCAL;
	memcpy(&instance->vswitch.sun_path, vswpath, MAX_NAME_LEN);
	if (connect(instance->mgmtfd, (struct sockaddr *)&instance->vswitch,
		    sizeof(struct sockaddr_un)) < 0) {
		perror("connect");
	}
}

int delete_vswitch_instance(struct vswitch_instance *instance)
{
	if (!instance) return -1;
	if (instance->mgmtfd <= 0) return -1;

	close(instance->mgmtfd);
	free(instance);

	return 0;
}

struct vswitch_instance *create_vswitch_instance(const char *instname)
{
	struct vswitch_instance *instance;
	char sockpath[MAX_NAME_LEN];
	int opt = 1;

	if (fam_unix_create_sock_path(instname))
		return NULL;
	fam_unix_get_sock_path(instname, 0, sockpath);

	instance = calloc(1, sizeof(struct vswitch_instance));
	if (!instance) return NULL;

	instance->mgmtfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if (instance->mgmtfd < 0) {
		perror("socket");
		goto __bail_free;
	}
	if (setsockopt(instance->mgmtfd, SOL_SOCKET, SO_REUSEADDR,
		       &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		goto __bail_close;
	}

	instance->sock.sun_family = PF_LOCAL;
	memcpy(&instance->sock.sun_path, sockpath, strlen(sockpath));

	if (bind(instance->mgmtfd, (struct sockaddr *)&instance->sock,
		 sizeof(instance->sock)) < 0) {
		perror("bind");
		goto __bail_close;
	}

	return instance;

__bail_close:
	close(instance->mgmtfd);
__bail_free:
	free(instance);
	return NULL;
}

ssize_t ping_vswitch(struct vswitch_instance *instance)
{
	char buf[32];
	ssize_t len;
	socklen_t sklen;

	/* vswitch does not ping itself, nor does the unconnected
	 * switch ping vswitch!
	 */
	if (!instance->vswitch.sun_path[0])
		return 0;

	strcpy(buf, "HELLO");
	sklen = sizeof(struct sockaddr_un);
	len = sendto(instance->mgmtfd, buf, strlen(buf), 0,
		     (struct sockaddr *)&instance->vswitch, sklen);
	if (len < 0) {
		perror("sendto");
	}

	return len;
}

ssize_t send_switch_topology_info(struct switchdev *dev, struct portconn *p, int nr_conn)
{
	struct tlv_object *obj = make_tlv(512); /* approx size */
	char *tlvobj;
	int i, len;

	push_tlv_object(obj,
			TLV_TTYPE_NOPARAM, TLV_T_TOPOLOGY,
			TLV_TTYPE_NOPARAM, TLV_T_PORTS,
			TLV_TTYPE_VALUE, TLV_TVALUE_U8, dev->nr_ports,
			TLV_TTYPE_NOPARAM, TLV_T_CONNECT,
			TLV_TTYPE_NONE);

	for (i = 0; i < nr_conn; i++) {
		push_tlv_object(obj,
				TLV_TTYPE_NOPARAM, TLV_T_PORTID,
				TLV_TTYPE_VALUE, TLV_TVALUE_U8, conn[i].srcport,
				TLV_TTYPE_NOPARAM, TLV_T_SW_NAME,
				TLV_TTYPE_DYNAMIC, strlen(conn[i].swname), conn[i].swname,
				TLV_TTYPE_NOPARAM, TLV_T_PORTID,
				TLV_TTYPE_VALUE, TLV_TVALUE_U8, conn[i].dstport,
				TLV_TTYPE_NONE);
	}

	tlvobj = get_tlv_object(obj, &len);
	if (!tlvobj) {
		printf("%s: TLV object encode error!\n");
	} else {
		dump_tlv_packet(tlvobj, len);
	}

	return 0;
}
