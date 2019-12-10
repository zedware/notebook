#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
A simple exam checker.

NOTICE:
1. The IDE is PyCharm.
2. The version is Python3.

TODO:
1. Finish the code for Windows.
"""
import filecmp
import os
import sys
import argparse

"""
Steps to test the program.

mkdir -p /tmp/exam/p1
cd /tmp/exam/p1
echo "#include <cstdio>                                                          
int main(void) {                                                           
  int c;                                                                   
  while ((c = fgetc(stdin)) != EOF) {                                      
    fputc(c, stdout);                                                      
  }                                                                        
  return 0;                                                                
}" > main.cc
echo "hello" > input.txt
echo "hello" > output.txt

mkdir -p /tmp/test/p1
cp /tmp/exam/p1/main.cc /tmp/test/p1/main.cc
"""

# Global variables
top_dir = '/tmp/exam'
compiler = ''
executable = 'exam'
build_file = 'build.txt'
input_file = 'input.txt'
output_file = 'output.txt'


def main():
    """
    The main loop.
    """

    # To override the default '-h' options with conflict handler 'resolve'.
    parser = argparse.ArgumentParser(description='Exam Checker',
                                     conflict_handler='resolve')
    parser.add_argument('-h', '--host', help='host', required=False)
    parser.add_argument('-P', '--port', help='port', required=False)
    args = parser.parse_args()

    # Set the default options.
    config = {'host': args.host if args.host is not None else '127.0.0.1',
              'port': args.port if args.port is not None else '3306'}

    global compiler
    if os.name == 'posix':
        _ = os.system('clear')
        compiler = "cc -Wall "
    elif os.name == 'nt':
        _ = os.system('cls')
        compiler = 'cl.exe '
    else:
        sys.stdout.write('Unknown os.name:' + os.name + '\n')
        sys.stdout.flush()
        return

    check('/tmp/test/p1/main.cc')
    return


def check(path=''):
    """
    `path` looks like /home/foo/p1/main.cc
    """

    if path == '':
        return False

    abspath = os.path.abspath(path)
    if not os.path.exists(abspath):
        sys.stdout.write(abspath + ' not found.\n')
        sys.stdout.flush()
        return False

    if not os.path.isfile(abspath):
        sys.stdout.write(abspath + ' not a file.\n')
        sys.stdout.flush()
        return False

    prefix = os.path.dirname(abspath)
    problem = os.path.basename(prefix)
    # file = os.path.basename(abspath)

    executable_abspath = prefix + os.sep + executable
    build_abspath = prefix + os.sep + build_file

    # Compile:
    cmdline = compiler + ' -o ' + executable_abspath + ' ' + abspath + ' ' + '1>' + build_abspath + ' 2>&1'
    os.system(cmdline)

    if not os.path.exists(executable_abspath):
        sys.stdout.write(abspath + ' build failed.\n')
        sys.stdout.flush
        return False

    input_abspath = prefix + os.sep + input_file
    output_abspath = prefix + os.sep + output_file

    # Run:
    cmdline = executable_abspath + ' <' + input_abspath + ' 1>' + output_abspath + ' 2>&1'
    os.system(cmdline)

    if not os.path.exists(output_abspath):
        sys.stdout.write(abspath + ' execute failed.\n')
        sys.stdout.flush
        return False

    expect_abspath = top_dir + os.sep + problem + os.sep + output_file
    if not os.path.isfile(expect_abspath):
        sys.stdout.write(expect_abspath + 'is missing.\n')
        sys.stdout.flush()
        return False

    # Diff:
    if not filecmp.cmp(expect_abspath, output_abspath):
        sys.stdout.write(abspath + ' result mismatch.\n')
        sys.stdout.flush
        return False

    sys.stdout.write(abspath + ' PASSED\n')
    sys.stdout.flush()
    return True


if __name__ == '__main__':
    main()
