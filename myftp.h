/*
File: myftp.cpp
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <typeinfo>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

int socket_connect(char *host, int port);
