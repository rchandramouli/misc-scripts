#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#include <json-c/json_inttypes.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>

#include <mysql.h>

#define MHD_SERVER_PORT       8800
#define MHD_POST_BUFFER_SIZE  32768

#define KEY_CONTENT_TYPE   "Content-Type"
#define KEY_CONTENT_JSON   "application/json"

#define MYSQL_DB_HOST      "localhost"
#define MYSQL_DB_USER      "testuser"
#define MYSQL_DB_PASSWORD  "testinst"
#define MYSQL_DB_NAMESPACE "testdb"

#define ARRAY_SIZE(x)    (sizeof(x)/sizeof(x[0]))

#define report_mysql_error_code(ctx, code)		\
    do {						\
	fprintf(stderr, "%s\n", mysql_error(ctx));	\
	return (code);					\
    } while (0)

#define report_mysql_error(ctx)	report_mysql_error_code(ctx, -1)

/* Globals */
static struct MHD_Daemon *g_daemon;
static MYSQL *g_mysql_ctx;

/*
 * JSON routines
 */
static int dump_keys (void *cls, enum MHD_ValueKind kind, 
		      const char *key, const char *value)
{
    printf("%s === %s\n", key, value);
    return MHD_YES;
}

static int check_json_content (void *cls, enum MHD_ValueKind kind, 
			       const char *key, const char *value)
{
    int *has_json = cls;

    if (strncmp(key, KEY_CONTENT_TYPE, strlen(KEY_CONTENT_TYPE)) == 0 &&
	strncmp(value, KEY_CONTENT_JSON, strlen(KEY_CONTENT_JSON)) == 0) {
	*has_json = 1;
    }
    return MHD_YES;
}

static const char *regn_fields[] = {
    "fname", "lname", "email", "phone", "add1", "add2", "add3", "country",
    "state", "pincode", "password",
};

#define MAX_REGN_FIELDS   ARRAY_SIZE(regn_fields)

struct _j2c_registration {
    const char *fields[MAX_REGN_FIELDS];
};

static const char *login_fields[] = {
    "email", "password",
};

#define MAX_LOGIN_FIELDS  ARRAY_SIZE(login_fields)

struct _j2c_login {
    const char *fields[MAX_LOGIN_FIELDS];
};

/*
 * MYSQL Client routines
 */

/*
 * Handle registration record. The regn record is a list of pointers
 * to the given JSON string. Note that in case if we are handling the
 * registration asynchronously, this needs to be changed. We assume
 * that the *regn would appropriately point to the buffer which is
 * valid.
 */
#if 0
| FirstName     | varchar(20) | YES  |     | NULL    |       |
| LastName      | varchar(20) | YES  |     | NULL    |       |
| Email         | varchar(40) | YES  |     | NULL    |       |
| Phone         | char(10)    | NO   | PRI | NULL    |       |
| Address1      | varchar(40) | YES  |     | NULL    |       |
| Address2      | varchar(40) | YES  |     | NULL    |       |
| Address3      | varchar(40) | YES  |     | NULL    |       |
| Country       | varchar(20) | YES  |     | NULL    |       |
| State         | varchar(20) | YES  |     | NULL    |       |
| Pincode       | varchar(6)  | YES  |     | NULL    |       |
| Password      | varchar(64) | YES  |     | NULL    |       |
| RegisteredOn  | date        | YES  |     | NULL    |       |
| LastUpdatedOn | date        | YES  |     | NULL    |       |
| Validated     | tinyint(1)  | YES  |     | NULL    |       |
+---------------+-------------+------+-----+---------+-------+
#endif

#define MAX_MYSQL_QRYSZ    1024

static int
mysql_handle_registration (struct _j2c_registration *jrecord)
{
    uint8_t qrybuf[MAX_MYSQL_QRYSZ];
    int len = 0;

    len += sprintf(qrybuf+len, "INSERT INTO AppUsers VALUES(");
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[0]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[1]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[2]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[3]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[4]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[5]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[6]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[7]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[8]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[9]);
    len += sprintf(qrybuf+len, "'%s',", jrecord->fields[10]);
    len += sprintf(qrybuf+len, "NULL, NULL, FALSE)");

    qrybuf[len] = '\0';

    if (mysql_query(g_mysql_ctx, qrybuf)) {
	report_mysql_error_code(g_mysql_ctx, MHD_NO);
    }

    return MHD_YES;
}

static int
mysql_handle_login (struct _j2c_login *jrecord)
{
    MYSQL_RES *result;
    uint8_t qrybuf[MAX_MYSQL_QRYSZ];
    int len = 0, ret = MHD_NO;

    len += sprintf(qrybuf+len, "SELECT * FROM AppUsers WHERE ");
    len += sprintf(qrybuf+len, "Email='%s' AND Password='%s'",
		   jrecord->fields[0], jrecord->fields[1]);

    qrybuf[len] = '\0';

    if (mysql_query(g_mysql_ctx, qrybuf)) {
	report_mysql_error_code(g_mysql_ctx, MHD_NO);
    }

    if ((result = mysql_store_result(g_mysql_ctx)) == NULL) {
	report_mysql_error_code(g_mysql_ctx, MHD_NO);
    }

    if (mysql_num_rows(result) == 1) {
	ret = MHD_YES;
    }

    mysql_free_result(result);

    return (ret);
}

static void
dump_regn_record (struct _j2c_registration *regn)
{
    int findex;

    for (findex = 0; findex < MAX_REGN_FIELDS; findex++) {
	printf("%s ===> %s\n", regn_fields[findex], regn->fields[findex]);
    }
}

static void
dump_login_record (struct _j2c_login *login)
{
    int findex;

    for (findex = 0; findex < MAX_LOGIN_FIELDS; findex++) {
	printf("%s ===> %s\n", login_fields[findex], login->fields[findex]);
    }
}

static int
json_parse_record_registration (struct json_object *jobj,
				struct _j2c_registration *regn)
{
    struct json_object *jfield;
    int findex;

    for (findex = 0; findex < MAX_REGN_FIELDS; findex++) {
	if (!json_object_object_get_ex(jobj, regn_fields[findex], &jfield)) {
	    printf("<skipping field '%s'>\n", regn_fields[findex]);
	    regn->fields[findex] = NULL;
	    continue;
	}
	regn->fields[findex] = json_object_get_string(jfield);
    }

    dump_regn_record(regn);
    
    return MHD_YES;
}

static int
json_parse_record_login (struct json_object *jobj,
			 struct _j2c_login *login)
{
    struct json_object *jfield;
    int findex;

    for (findex = 0; findex < MAX_LOGIN_FIELDS; findex++) {
	if (!json_object_object_get_ex(jobj, login_fields[findex], &jfield)) {
	    printf("<skipping field '%s'>\n", login_fields[findex]);
	    login->fields[findex] = NULL;
	    continue;
	}
	login->fields[findex] = json_object_get_string(jfield);
    }
    dump_login_record(login);

    return MHD_YES;
}

static int
json_post_handler (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
		   const char *filename, const char *content_type,
		   const char *transfer_encoding, const char *data, uint64_t off,
		   size_t size)
{
    struct json_object *jobj = json_tokener_parse(data);
    struct json_object *jhdr;
    struct _j2c_registration regn_record;
    struct _j2c_login login_record;
    int ret;

    if (!jobj) {
	return MHD_NO;
    }

    if (json_object_object_get_ex(jobj, "registration", &jhdr)) {
	memset(&regn_record, 0, sizeof(regn_record));
	if (json_parse_record_registration(jhdr, &regn_record)) {
	    if (ret = mysql_handle_registration(&regn_record)) {
		return MHD_YES;
	    }
	}

    } else if (json_object_object_get_ex(jobj, "login", &jhdr)) {
	memset(&login_record, 0, sizeof(login_record));
	if (json_parse_record_login(jhdr, &login_record)) {
	    if (ret = mysql_handle_login(&login_record)) {
		return MHD_YES;
	    } else {
		fprintf(stderr, "%s: LOGIN AUTH FAILURE!\n", __func__);
	    }
	}
    }	
	    
    return MHD_NO;
}

/*
 * Microhttpd connection handler for all type of messages
 */
static int mhd_connection_handler (void *cls,
				   struct MHD_Connection *connection,
				   const char *url,
				   const char *method,
				   const char *version,
				   const char *upload_data,
				   size_t *upload_data_size,
				   void **con_cls)
{
    const char *page = "<html><body>Hello, browser!</body></html>";
    struct MHD_Response *resp;
    struct MHD_PostProcessor *pp = *con_cls;
    int ret = MHD_NO, has_json;

    printf("URL = <%s>\n", url);
    printf("METHOD = <%s>\n", method);
    printf("VERSION = <%s>\n", version);
    printf("UPLOAD_DATA = <%s>\n SIZE = %zd\n",
	   upload_data, *upload_data_size);
    printf("pp = %p\n\n", pp);

    if (pp != NULL) {
	if (*upload_data_size == 0) {
	    MHD_destroy_post_processor(pp);
	} else {
	    printf("UPLOAD DATA:\n<%s>\n DATA-SIZE: %zd\n", upload_data,
		   *upload_data_size);
	    MHD_post_process(pp, upload_data, *upload_data_size);
	    *upload_data_size = 0;
	    return MHD_YES;
	}
    } else {
	has_json = 0;
	MHD_get_connection_values(connection, MHD_HEADER_KIND, &check_json_content, &has_json);
	if (has_json) {
	    printf("FOUND JSON content!\n");
	    pp = MHD_create_post_processor(connection, MHD_POST_BUFFER_SIZE,
					   &json_post_handler, NULL);
	    if (pp == NULL) {
		printf("%s: creating post processor failed!!!\n", __func__);
	    }
	    *con_cls = pp;
	    return MHD_YES;
	}
    }
    resp = MHD_create_response_from_buffer(strlen(page), (void *)page,
					   MHD_RESPMEM_PERSISTENT);

    ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);

    return ret;
}

static int
close_mysql_connection (void)
{
    if (g_mysql_ctx) {
	mysql_close(g_mysql_ctx);
    }
    return 0;
}

static int
open_mysql_connection (void)
{
    fprintf(stdout, "Initializing schema, MySQL version = %s\n",
	    mysql_get_client_info());

    if ((g_mysql_ctx = mysql_init(NULL)) == NULL) {
	report_mysql_error(g_mysql_ctx);
    }

    if (mysql_real_connect(g_mysql_ctx, MYSQL_DB_HOST, MYSQL_DB_USER,
			   MYSQL_DB_PASSWORD, MYSQL_DB_NAMESPACE,
			   0, NULL, CLIENT_MULTI_STATEMENTS) == NULL) {
	report_mysql_error(g_mysql_ctx);
    }

    fprintf(stdout, "MySQL connected!\n");

    return 0;
}

static void
sig_kill_handler (int signum)
{
    fprintf(stderr, "%s: killing daemon!\n", __func__);

    if (g_daemon)
	MHD_stop_daemon(g_daemon);

    close_mysql_connection();

    exit(0);
}

/*
 * Start the webserver and listen to given port
 */
int main (int argc, char *argv[])
{
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
			      MHD_SERVER_PORT,
			      NULL,
			      NULL,
			      &mhd_connection_handler,
			      NULL,
			      MHD_OPTION_EXTERNAL_LOGGER,
			      fprintf,
			      MHD_OPTION_END);
    if (daemon == NULL) {
	fprintf(stderr, "unable to start MHD daemon on port %d\n",
		MHD_SERVER_PORT);
	return -1;
    }

    /* Initialize MySQL */
    if (open_mysql_connection()) {
	fprintf(stderr, "opening MYSQL connection failed\n");
	if (g_daemon) {
	    MHD_stop_daemon(g_daemon);
	}
	return -1;
    }
    

    g_daemon = daemon;

    signal(SIGUSR1, sig_kill_handler);

    while (1) {
	sleep(60);
    }

    return 0;
}

/*
 * Local Variables:
 * compile-command:"gcc handle_response_microhttpd.c -I../production/include -L../production/lib `mysql_config --cflags` -lmicrohttpd -ljson-c `mysql_config --libs`"
 * End:
 */
