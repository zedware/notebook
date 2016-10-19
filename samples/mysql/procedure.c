//
// Based on the samples from MySQL's document. Works on Linux and Window.
//
#ifdef WIN32
#include <tchar.h>
#include <WinSock2.h>
#endif // WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysql.h"

void test_error (MYSQL *mysql, int status)
{
    if (status != 0 && status != MYSQL_NO_DATA) {
        printf ("%d: %s\n", mysql_errno (mysql), mysql_error (mysql));
    }
}

void test_stmt_error (MYSQL_STMT *stmt, int status)
{
    if (status != 0 && status != MYSQL_NO_DATA) {
        printf ("%d: %s\n", mysql_stmt_errno (stmt), mysql_stmt_error (stmt));
    }
}

void sp()
{
    const char *host = "127.0.0.1";
    const char *user = "root";
    const char *password = NULL;
    const char *database = "test";
    unsigned int port = 3306;
    MYSQL      MySQL, *mysql;
    MYSQL_STMT *stmt;
    MYSQL_BIND ps_params[3];  /* input parameter buffers */
    int        int_data[3];   /* input/output values */
    char       string_data[3][100];
    my_bool    is_null[3];    /* output value nullability */
    int        status;

    mysql_init (&MySQL);
    mysql = mysql_real_connect (&MySQL, host, user, password, database, port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS);

    /* set up stored procedure */
    status = mysql_query (mysql, "DROP PROCEDURE IF EXISTS p1");
    test_error (mysql, status);

    status = mysql_query (mysql,
                          "CREATE PROCEDURE p1("
                          "  IN p_in INT, "
                          "  OUT p_out INT, "
                          "  INOUT p_inout INT) "
                          "BEGIN "

                          "DECLARE done INT DEFAULT FALSE; "
                          "DECLARE v1, w1 INT; "
                          "DECLARE v2, w2 CHAR(8); "
                          "DECLARE cur1 CURSOR FOR SELECT c1, c2 FROM t1; "
                          "DECLARE cur2 CURSOR FOR SELECT c1, c2 FROM t2; "
                          "DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE; "

                          "  SELECT p_in, p_out, p_inout; "
                          "  SET p_in = 100, p_out = 200, p_inout = 300; "
                          "  SELECT p_in, p_out, p_inout; "
                          "  SELECT c1 FROM t1; "

                          "OPEN cur1; "
                          "OPEN cur2; "
                          "read_loop: LOOP "
                          "FETCH cur1 INTO v1, v2; "
                          "FETCH cur2 INTO w1, w2; "
                          "  IF done THEN "
                          "   LEAVE read_loop; "
                          "  END IF; "
                          "IF v1 > w1 THEN "
                          " SELECT '>'; "
                          "ELSE "
                          " SELECT '<='; "
                          "END IF; "
                          "END LOOP; "
                          " "
                          "CLOSE cur1; "
                          "CLOSE cur2; "
                          "END");
    test_error (mysql, status);

    /* initialize and prepare CALL statement with parameter placeholders */
    stmt = mysql_stmt_init (mysql);

    if (!stmt) {
        printf ("Could not initialize statement\n");
        exit (1);
    }

    status = mysql_stmt_prepare (stmt, "CALL p1(?, ?, ?)", 16);
    test_stmt_error (stmt, status);

    /* initialize parameters: p_in, p_out, p_inout (all INT) */
    memset (ps_params, 0, sizeof (ps_params));

    ps_params[0].buffer_type = MYSQL_TYPE_LONG;
    ps_params[0].buffer = (char *)&int_data[0];
    ps_params[0].length = 0;
    ps_params[0].is_null = 0;

    ps_params[1].buffer_type = MYSQL_TYPE_LONG;
    ps_params[1].buffer = (char *)&int_data[1];
    ps_params[1].length = 0;
    ps_params[1].is_null = 0;

    ps_params[2].buffer_type = MYSQL_TYPE_LONG;
    ps_params[2].buffer = (char *)&int_data[2];
    ps_params[2].length = 0;
    ps_params[2].is_null = 0;

    /* bind parameters */
    status = mysql_stmt_bind_param (stmt, ps_params);
    test_stmt_error (stmt, status);

    /* assign values to parameters and execute statement */
    int_data[0] = 10;  /* p_in */
    int_data[1] = 20;  /* p_out */
    int_data[2] = 30;  /* p_inout */

    status = mysql_stmt_execute (stmt);
    test_stmt_error (stmt, status);

    /* process results until there are no more */
    do {
        int i;
        int num_fields;       /* number of columns in result */
        MYSQL_FIELD *fields;  /* for result set metadata */
        MYSQL_BIND *rs_bind;  /* for output buffers */

        /* the column count is > 0 if there is a result set */
        /* 0 if the result is only the final status packet */
        num_fields = mysql_stmt_field_count (stmt);

        if (num_fields > 0) {
            /* there is a result set to fetch */
            printf ("Number of columns in result: %d\n", (int)num_fields);

            /* what kind of result set is this? */
            printf ("Data: ");

            if (mysql->server_status & SERVER_PS_OUT_PARAMS)
                printf ("this result set contains OUT/INOUT parameters\n");
            else
                printf ("this result set is produced by the procedure\n");

            MYSQL_RES *rs_metadata = mysql_stmt_result_metadata (stmt);
            test_stmt_error (stmt, rs_metadata == NULL);

            fields = mysql_fetch_fields (rs_metadata);

            rs_bind = (MYSQL_BIND *)malloc (sizeof (MYSQL_BIND) * num_fields);

            if (!rs_bind) {
                printf ("Cannot allocate output buffers\n");
                exit (1);
            }

            memset (rs_bind, 0, sizeof (MYSQL_BIND) * num_fields);

            /* set up and bind result set output buffers */
            for (i = 0; i < num_fields; ++i) {
                rs_bind[i].buffer_type = fields[i].type;
                rs_bind[i].is_null = &is_null[i];

                switch (fields[i].type) {
                case MYSQL_TYPE_LONG:
                    rs_bind[i].buffer = (char *) & (int_data[i]);
                    rs_bind[i].buffer_length = sizeof (int_data);
                    break;

                case MYSQL_TYPE_STRING:
                case MYSQL_TYPE_VAR_STRING:
                    rs_bind[i].buffer = (char *) (string_data[i]);
                    rs_bind[i].buffer_length = 100;
                    break;

                default:
                    fprintf (stderr, "ERROR: unexpected type: %d.\n", fields[i].type);
                    break;
                }
            }

            status = mysql_stmt_bind_result (stmt, rs_bind);
            test_stmt_error (stmt, status);

            /* fetch and display result set rows */
            while (1) {
                status = mysql_stmt_fetch (stmt);
                test_stmt_error (stmt, status);

                if (status == 1 || status == MYSQL_NO_DATA)
                    break;

                for (i = 0; i < num_fields; ++i) {
                    switch (rs_bind[i].buffer_type) {
                    case MYSQL_TYPE_LONG:
                        if (*rs_bind[i].is_null)
                            printf (" val[%d] = NULL;", i);
                        else
                            printf (" val[%d] = %ld;", i, (long) * ((int *)rs_bind[i].buffer));

                        break;

                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_VAR_STRING:
                        if (*rs_bind[i].is_null)
                            printf (" val[%d] = NULL;", i);
                        else
                            printf (" val[%d] = %s;", i, (char *) (rs_bind[i].buffer));

                        break;

                    default:
                        printf (" unexpected type (%d)\n", rs_bind[i].buffer_type);
                    }
                }

                printf ("\n");
            }

            mysql_free_result (rs_metadata); /* free metadata */
            free (rs_bind);                  /* free output buffers */
        } else {
            /* no columns = final status packet */
            printf ("End of procedure output\n");
        }

        /* more results? -1 = no, >0 = error, 0 = yes (keep looking) */
        status = mysql_stmt_next_result (stmt);

        if (status > 0) {
            test_stmt_error (stmt, status);
        }
    } while (status == 0);

    mysql_stmt_close (stmt);
    mysql_close (mysql);
}

#ifdef WIN32
int _tmain (int argc, _TCHAR *argv[])
{
#else
int main (int argc, char *argv[])
{
#endif // WIN32
    sp();
    return 0;
}
