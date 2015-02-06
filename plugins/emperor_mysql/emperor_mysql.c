
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
	
	if(mysql_query(conn, "SELECT * FROM vassals")) {
		uwsgi_log("[emperor] %s\n", mysql_error(conn));
		goto end;
	}
	
end:
	uwsgi_log("[emperor] MySQL disconnecting\n");
	mysql_close(conn);
}


struct uwsgi_plugin emperor_mysql_plugin = {
	.name = "emperor_mysql",
	.on_load = emperor_mysql_init,
};
