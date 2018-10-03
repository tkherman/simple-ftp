/*
File: myftp.h
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1, jhardey
*/

#include "../network_utils/network_utils.h"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <typeinfo>
#include <sys/time.h>

enum Operation {
    DL,
    UP,
    RM,
    LS,
    MKDIR,
    RMDIR,
    CD,
    EXIT,
    UNKNOWN
};

int socket_connect(char *host, int port);
Operation parse_input(std::vector<std::string> &arguments);
void upload_file (int sockfd, std::vector<std::string> args);
void get_ls(int sockfd);
void make_directory(int sockfd, std::vector<std::string> args);
void remove_file(int sockfd, std::vector<std::string> args);
void download_file(int sockfd, std::vector<std::string> args);
void change_directory(int sockfd, std::vector<std::string> args);
