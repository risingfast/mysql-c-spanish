# -*- Makdfile -*-
# Created: 04/10/2020
# Geoffrey Jarman
# makefile for C project -- mysql-c-spanish

# $@ Target file
# $^ Dependency files
# $(CC) Compiler executable
# $(CFLAGS) Compiler options
# $(SQL1FLAGS) Include file for mysql header
# $(SQL2FLAGS) mysql libraries

# Variables

CC=gcc
CFLAGS=-g -o
SQL1FLAGS=-I/usr/include/mysql
SQL2FLAGS=-L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl -lresolv

all: mysql-c-spanish

mysql-c-spanish: mysql-c-spanish.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

clean:
	rm -f *.o *.s *.i mysql-c-spanish
