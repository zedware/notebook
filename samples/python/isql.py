#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
A simple interactive MySQL client written in python.

To install the MySQL Connector/Python, 
 $ pip3 install mysql-connector
http://dev.mysql.com/doc/connector-python/
"""

from os import system, name
import sys
import argparse
import re
import mysql.connector


def main():
    """
    The main loop to process the statements.
    """

    # To override the default '-h' options with conflict handler 'resolve'.
    parser = argparse.ArgumentParser(description='Interactive MySQL client',
                                     conflict_handler='resolve')
    parser.add_argument('-h', '--host', help='host', required=False)
    parser.add_argument('-P', '--port', help='port', required=False)
    parser.add_argument('-u', '--user', help='user', required=False)
    parser.add_argument('-p', '--password', help='password', required=False)
    parser.add_argument('-D', '--database', help='database', required=False)
    args = parser.parse_args()

    # Set the default options.
    config = {'host': args.host if args.host is not None else '127.0.0.1',
              'port': args.port if args.port is not None else '3306',
              'user': args.user if args.user is not None else 'root',
              'password': args.password if args.password is not None else '',
              'database': args.database if args.database is not None else 'test', 'use_unicode': True,
              'charset': 'utf8'}

    cnx = mysql.connector.connect(**config)
    if name == 'posix':
        _ = system('clear')
    elif name == 'nt':
        _ = system('cls')
    else:
        sys.stdout.write('Unknown os.name:' + name + '\n')
        sys.stdout.flush()

    while True:
        cur = cnx.cursor()
        line = ''
        single_line = True
        while True:
            if single_line:
                sys.stdout.write("mysql> ")  # waiting for input
            else:
                sys.stdout.write("    -> ")  # waiting for a delimeter (;)

            sys.stdout.flush()
            line += sys.stdin.readline()

            if single_line:
                a_line = line.rstrip('\n')
                if a_line == r'quit' or a_line == r'\q':
                    return
                single_line = False

            # Match a delimiter (;) at the end of the string
            if re.search(r';\s*$', line):
                break

        try:
            cur.execute(line)
            for row in cur.fetchall():
                sys.stdout.write(repr(row) + '\n')

        except mysql.connector.errors.InterfaceError as err1:
            # not used
            _ = err1
            continue

        except (mysql.connector.errors.ProgrammingError,
                mysql.connector.errors.DatabaseError) as err2:
            sys.stdout.write(str(err2) + '\n')
            continue

        cur.close()

    cnx.close()
    return


if __name__ == '__main__':
    main()
