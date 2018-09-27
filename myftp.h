/*
File: myftp.h
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1
*/

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
