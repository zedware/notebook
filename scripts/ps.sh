#!/bin/bash

ps -ef | grep mysqld | grep '[45][01]06' | grep -v grep

