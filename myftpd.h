/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa O'Sorio
 * NetId: ktong1, jhardey, josorio2
 *
 * myftpd.h
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 1


int socket_bind_listen(int port);
