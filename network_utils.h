/*
 * File: network_utils.h
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 */

#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* 
 * Send and receive any std::string.
 * It first sends the length of string then the actualy
 * msg.
 * recv_string function will store the string in msg that
 * is passed by reference
 */
int send_string(int sockfd, std::string msg);
int recv_string(int sockfd, std::string &msg);

/*
 * Send the filename length and then the actual filename
 */
int send_filename(int sockfd, std::string filename);
int recv_filename(int sockfd, std::string &filename);

/*
 * Send 32-bit file size
 */
int send_file_size(int sockfd, uint32_t size);
int recv_file_size(int sockfd, uint32_t &size);

std::string get_file_md5(std::string filename);
