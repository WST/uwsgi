
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
	
	uwsgi_log("connecting to MySQL %s\n", conn_string);
	
	if(conn) mysql_close(conn);
}


struct uwsgi_plugin emperor_mysql_plugin = {
	.name = "emperor_mysql",
	.on_load = emperor_mysql_init,
};
