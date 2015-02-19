
#include "../../uwsgi.h"
#include <my_global.h>
#include <mysql.h>

extern struct uwsgi_server uwsgi;
extern struct uwsgi_instance *ui;

void uwsgi_imperial_monitor_mysql_init(struct uwsgi_emperor_scanner *);
void uwsgi_imperial_monitor_mysql(struct uwsgi_emperor_scanner *);
void emperor_mysql_init(void);

void emperor_mysql_init(void) {
	uwsgi_register_imperial_monitor("mysql", uwsgi_imperial_monitor_mysql_init, uwsgi_imperial_monitor_mysql);
}

void uwsgi_imperial_monitor_mysql_init(struct uwsgi_emperor_scanner *ues) {
	uwsgi_log("[emperor] enabled emperor MySQL monitor\n");
}

void uwsgi_imperial_monitor_mysql(struct uwsgi_emperor_scanner *ues) {
	MYSQL *conn = mysql_init(NULL);

	char *conn_string = uwsgi_str(ues->arg + 8);
	
	uwsgi_log("[emperor] connecting to MySQL (group %s should exist in my.cnf)\n", conn_string);
	
	mysql_options(conn, MYSQL_READ_DEFAULT_GROUP, conn_string);
	
	if(!mysql_real_connect(conn, NULL, NULL, NULL, NULL, 0, NULL, 0)) {
		uwsgi_log("[emperor] %s\n", mysql_error(conn));
		goto end;
	}
	
	char *query = uwsgi.emperor_tyrant ? "SELECT id, name, config FROM vassals" : "SELECT id, name, config FROM vassals";
	
	if(mysql_query(conn, query)) {
		uwsgi_log("[emperor] %s\n", mysql_error(conn));
		goto end;
	}
	
	unsigned int cols = mysql_field_count(conn);
	if((cols != 3) && (cols != 5)) {
		uwsgi_log("[emperor] SQL query should return 3 or 5 columns, but not %d", cols);
		goto end;
	}
	
	MYSQL_RES *result = mysql_store_result(conn);
	
	MYSQL_ROW row;
	while((row = mysql_fetch_row(result))) {
		char *name = row[1];
		char *config = row[2];
		
		uid_t vassal_uid = 0;
		gid_t vassal_gid = 0;
		
		if (uwsgi.emperor_tyrant) {
			if (cols != 5) {
				uwsgi_log("[emperor] missing uid and gid for vassal %s\n", name);
				continue;
			}
			char *q_uid = row[3];
			char *q_gid = row[4];
			
			vassal_uid = uwsgi_str_num(q_uid, strlen(q_uid));
			vassal_gid = uwsgi_str_num(q_gid, strlen(q_gid));
		}
		
		char *socket_name = NULL;
		uwsgi_emperor_simple_do(ues, name, config, 0, vassal_uid, vassal_gid, socket_name);
	}
	
	// TODO: check for removed instances
	
end:
	uwsgi_log("[emperor] MySQL disconnecting\n");
	mysql_close(conn);
}


struct uwsgi_plugin emperor_mysql_plugin = {
	.name = "emperor_mysql",
	.on_load = emperor_mysql_init,
};
