/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa O'Sorio
 * NetId: ktong1, jhardey, josorio2
 *
 * myftpd.h
 */

#include "../network_utils/network_utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 1

/* server operations */
void receive_upload_file(int sockfd);

/* server set up */
int socket_bind_listen(int port);
int accept_connection(int sockfd);
void handle_ftp_requests(int newfd);
void send_download_file(int sockfd);
