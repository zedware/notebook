#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
A simple interactive MySQL client written in python.

To install the MySQL Connector/Python, visit:
http://dev.mysql.com/doc/connector-python/en/connector-python-installation-binary.html
"""

import sys
import argparse
import re
import mysql.connector

def main():
    """
    The main loop to process the statements.
    """

    # To overide the default '-h' options with conflict handler 'resolve'.
    parser = argparse.ArgumentParser(description='Interactive MySQL client',
                                     conflict_handler='resolve')
    parser.add_argument('-h', '--host', help='host', required=False)
    parser.add_argument('-P', '--port', help='port', required=False)
    parser.add_argument('-u', '--user', help='user', required=False)
    parser.add_argument('-p', '--password', help='password', required=False)
    parser.add_argument('-D', '--database', help='database', required=False)
    args = parser.parse_args()

    # Set the default options.
    config = {
        'host': '',
        'port': '',
        'user': '',
        'password': '',
        'database': '',
        'use_unicode': True,
        'charset': 'utf8'
    }

    config['host'] = args.host if args.host is not None else '127.0.0.1'
    config['port'] = args.port if args.port is not None else '3306'
    config['user'] = args.user if args.user is not None else 'root'
    config['password'] = args.password if args.password is not None else ''
    config['database'] = args.database if args.database is not None else 'test'

    cnx = mysql.connector.connect(**config)

    while True:
        cur = cnx.cursor()
        line = ''
        single_line = True
        while True:
            if single_line:
                sys.stdout.write("mysql> ")  # waiting for input
            else:
                sys.stdout.write("    -> ")  # waiting for a delimeter (;)

            line += sys.stdin.readline()

            if single_line:
                a_line = line.rstrip('\n')
                if a_line == r'quit' or a_line == r'\q':
                    return
                single_line = False

            # Match a delimeter (;) at the end of the string
            if re.search(r';\s*$', line):
                break

        try:
            cur.execute(line)
            for row in cur.fetchall():
                sys.stdout.write(repr(row) + '\n')

        except (mysql.connector.errors.InterfaceError) as err1:
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

