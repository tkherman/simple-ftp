/*
 * File: client_operation.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 *
 */

#include "myftp.h"


void upload_file (int sockfd, std::vector<std::string> args) {
    struct stat file_stat;
    std::string filename;
    std::string ret_filename;
    char buffer[BUFSIZ];

    // Check if input file exists
    if (args.size() != 1) {
        std::cerr << "No input file specified" << std::endl;
        return;
    }
    filename = args[0];

    if (stat(filename.c_str(), &file_stat) != 0) {
        std::cerr << "File specified: " << filename << " doesn't exist" << std::endl;
        return;
    }
    
    // Send UP request to server
    if (send_string(sockfd, std::string("UP")) < 0) {
        std::cerr << "ERROR send UP request" << std::endl;
        return;
    }
    
    // Send filename to server
    if (send_filename(sockfd, filename) < 0) {
        std::cerr << "Client fails to send filename to server" << std::endl;
        return;
    }
    
    // Recv repeated filename from server, if it fails or it doesn't match
    // return
    if (recv_filename(sockfd, ret_filename) < 0 || ret_filename.compare(filename)) {
        std::cerr << "Filename returned from server doesn't match" << std::endl;
        return;
    }

    // Send file size
    if (send_file_size(sockfd, (uint32_t)file_stat.st_size) < 0) {
        std::cerr << "Client fails to send file size to server" << std::endl;
        return;
    }

    std::cout << get_file_md5(filename) << std::endl;
    std::cout << file_stat.st_size << std::endl;
}
