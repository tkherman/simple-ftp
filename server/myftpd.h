/*
 * Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
 * NetId: ktong1, jhardey, josorio2
 *
 * myftpd.h
 */

#include "../network_utils/network_utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 1

/* server operations */
void receive_upload_file(int sockfd);
std::string get_dir_listing();
void service_ls_request(int sockfd);

/* server set up */
int socket_bind_listen(int port);
int accept_connection(int sockfd);
void handle_ftp_requests(int newfd);
