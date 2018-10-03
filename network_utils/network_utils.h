/*
 * File: network_utils.h
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 */

#include <string>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <fstream>

#include <dirent.h>
#include <sys/time.h>
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

/*
 * Send and receive binary files
 */
int send_file(int sockfd, std::string filename);
int recv_file(int sockfd, uint32_t size, std::string filename);

/*
 * Generate the md5 hash
 */
std::string get_file_md5(std::string filename);

/*
 * Check if directory exist and is empty
 * Returns 1 if exist and empty
 * Returns 0 if not empty
 * returns -1 if doesn't exist
 * https://stackoverflow.com/questions/6383584/check-if-a-directory-is-empty-using-c-on-linux
 */
int get_dir_info(std::string dir_name);

/*
 * Get time elasped in microseconds
 * Take 2 struct timeeval as input
 */
int get_time_elasped(struct timeval start_time, struct timeval end_time);
