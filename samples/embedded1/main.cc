#include <mysql.h>
#include <iostream>
#include <cassert>

using namespace std;

const char *server_options[] = \
       { "mysql_test", "--defaults-file=my.cnf", NULL };
int num_elements = (sizeof(server_options) / sizeof(char *)) - 1;

const char *server_groups[]= { "libmysqld_server",
                               "libmysqld_client",
                               NULL};

bool is_server_started= false;

MYSQL *MySQL= NULL;

const char *db= NULL;

void start_server()
{
  cout << "enter start_server()" << endl;
  if (mysql_library_init(num_elements, (char **) server_options, (char **) server_groups)) {
    is_server_started= false;
    cout << "ERROR: start server failed." << endl;
  }
  else {
    is_server_started= true;
    cout << "INFO: start server OK." << endl;
  }
}

void stop_server()
{
  cout << "enter stop_server()" << endl;
  mysql_server_end();
}

bool connect_server()
{
  cout << "enter connect_server()" << endl;
  bool rc = true;

  MySQL = mysql_init(NULL);
  if (!MySQL)
    return rc;

  if (mysql_real_connect(MySQL, NULL, NULL, NULL, db, 0, NULL, 0))
    rc = false;
 
  MySQL->reconnect= 1;
  return rc;
}

void output_rows(MYSQL_RES *res)
{
  MYSQL_ROW row;
  unsigned long n = 0;

  while ((row= mysql_fetch_row(res)) != 0)
  {
    mysql_field_seek(res, 0);
    for (unsigned int i= 0 ; i < mysql_num_fields(res); i++)
      cout << "row[" << n++ << "]:"<< row[i] << endl; 
  }
}

bool get_dbs()
{
  MYSQL_RES *res;

  if (!is_server_started)
    return true;

  if (!(res= mysql_list_dbs(MySQL, "%")))
    return true;

  output_rows(res);
  mysql_free_result(res);
  return false;
}

int main(int argc, char *argv[])
{
  start_server();
  if (is_server_started) {
    cout << "server started." << endl;
    if (!connect_server()) {
      get_dbs();
    }
    stop_server();
  }

  return 0;
}

