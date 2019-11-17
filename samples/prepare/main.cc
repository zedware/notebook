// Brain damaged semantic used by MySQL.
// true   - error
// false  - ok

#include <mysql.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>

using namespace std;

int test_prepare= 0;

#ifdef TEST_EMBEDDED_SERVER
const char *server_options[]= \
       { "mysql_test", "--defaults-file=my.cnf", NULL };

const char *server_groups[]= { "libmysqld_server",
                               "libmysqld_client",
                               NULL};
const char *host= NULL;
const char *user= NULL;
const char *passwd= NULL;
const char *db= NULL;
unsigned int port= 0;
#else

const char *server_options[]= { NULL };
const char *server_groups[]= { NULL };
const char *host= "127.0.0.1";
const char *user= "root";
const char *passwd= "";
const char *db= "test";
unsigned int port= 4006;
#endif

int num_elements= (sizeof(server_options) / sizeof(char *)) - 1;

bool is_server_started= false;

MYSQL *MySQL= NULL;

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
  cout << "leave start_server()" << endl;
}

void stop_server()
{
  cout << "enter stop_server()" << endl;
  mysql_server_end();
  cout << "leave stop_server()" << endl;
}

bool connect_server()
{
  cout << "enter connect_server()" << endl;
  bool rc= true;

  MySQL= mysql_init(NULL);
  if (!MySQL)
    return rc;

  // in seconds
  unsigned int read_timeout= 3;

  // options should be set before connect.
  mysql_options(MySQL, MYSQL_OPT_READ_TIMEOUT, (const char *) &read_timeout);

  if (mysql_real_connect(MySQL, host, user, passwd, db, port, NULL, 0)) {
    rc= false;
  }
 
  MySQL->reconnect= 1;
  cout << "leave connect_server()" << endl;
  return rc;
}

void output_rows(MYSQL_RES *res)
{
  MYSQL_ROW row;
  unsigned long n= 0;

  while ((row= mysql_fetch_row(res)) != 0)
  {
    mysql_field_seek(res, 0);
    for (unsigned int i= 0 ; i < mysql_num_fields(res); i++)
      cout << "row[" << n++ << "]:"<< row[i] << endl; 
  }
}

bool get_dbs()
{
  cout << "enter get_dbs()" << endl;
  MYSQL_RES *res;

  if (!(res= mysql_list_dbs(MySQL, "%")))
    return true;

  output_rows(res);
  mysql_free_result(res);
  cout << "leave get_dbs()" << endl;
  return false;
}

static MYSQL_STMT *STDCALL
mysql_simple_prepare(MYSQL *mysql_arg, const char *query)
{
 MYSQL_STMT *stmt= mysql_stmt_init(mysql_arg);
 if (stmt && mysql_stmt_prepare(stmt, query, (uint) strlen(query)))
 {
   mysql_stmt_close(stmt);
   return 0;
 }
 return stmt;
}

bool do_query()
{
  cout << "enter do_query()" << endl;

  if (!test_prepare)
  {
      const char *sql1= "select host, user from mysql.user limit 3";
      const char *sql2= "insert into foo values(1, 'john')";
      const char *sql= "select * from foo where id=1";
      while (1) 
      {
        my_ulonglong affected_rows;

        cout << "execute query..." << endl;
        if (mysql_query(MySQL, sql)) {
          cout << mysql_error(MySQL) << endl;
          return true;
        }

        affected_rows = mysql_affected_rows(MySQL);
        cout << "affected_rows: " << (long) affected_rows << endl;

        MYSQL_RES *res= mysql_use_result(MySQL);
        if (res) {
          output_rows(res);
          mysql_free_result(res);
        }
      }
  }
  else
  {
      int rc;
      MYSQL_STMT *ps;
      MYSQL_BIND psp;
      my_bool psp_null;
      int pspv;

      ps = mysql_simple_prepare(MySQL, "select * from foo where id=?");
      if (ps == NULL)
        cout << "prepare failed" << endl;
      else
      {
        while(1) {
          memset(&psp, 0, sizeof (psp)); 
          psp.buffer_type= MYSQL_TYPE_LONG;
          psp.is_null= &psp_null;
          psp.buffer= (char *) &pspv;
          psp.buffer_length= sizeof (psp); 
          
          pspv = 1;
          rc= mysql_stmt_bind_param(ps, &psp);
          if (rc != 0)
            cout << "bind failed" << endl;
          else 
          {
            rc= mysql_stmt_execute(ps); 
            if (rc != 0)
              cout << "execute failed" << endl;
          }
        }
      }
  }

  cout << "leave do_query()" << endl;
  return false; 
}

int main(int argc, char *argv[])
{
  if (argc == 2 && argv[1][0] == 'p')
  {
    test_prepare= 1;
    cout << "test_prepare= 1" << endl;
  }

  start_server();
  if (is_server_started) {
    cout << "server started." << endl;

    if (!connect_server()) {
      get_dbs();
      do_query();
    }

    stop_server();
  }

  return 0;
}

