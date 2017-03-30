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

#include <mysql.h>

#define DEBUG
#include <gweb/common.h>
#include <gweb/server.h>
#include <gweb/json_api.h>
#include <gweb/mysqldb_api.h>

/* Globals */
static struct MHD_Daemon *g_daemon;

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

static int
json_post_handler (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
		   const char *filename, const char *content_type,
		   const char *transfer_encoding, const char *data, uint64_t off,
		   size_t size)
{
    if (gweb_json_post_processor(data, size)) {
	log_error("JSON post processor failed to handle API\n");
	return MHD_NO;
    }
    return MHD_YES;
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
	    pp = MHD_create_post_processor(connection, GWEB_POST_BUFSZ,
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

static void
sig_kill_handler (int signum)
{
    fprintf(stderr, "%s: killing daemon!\n", __func__);

    if (g_daemon)
	MHD_stop_daemon(g_daemon);

    gweb_mysql_shutdown();

    exit(0);
}

/*
 * Start the webserver and listen to given port
 */
int main (int argc, char *argv[])
{
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
			      GWEB_SERVER_PORT,
			      NULL,
			      NULL,
			      &mhd_connection_handler,
			      NULL,
			      MHD_OPTION_EXTERNAL_LOGGER,
			      fprintf,
			      MHD_OPTION_END);
    if (daemon == NULL) {
	fprintf(stderr, "unable to start MHD daemon on port %d\n",
		GWEB_SERVER_PORT);
	return -1;
    }

    /* Initialize MySQL */
    if (gweb_mysql_init()) {
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
