/*
 * File: client_operation.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 *
 */

#include "myftp.h"

void upload_file (int sockfd, std::vector<std::string> args) {
    struct stat file_stat;
    uint16_t filename_len;
    std::string filename;
    char buffer[BUFSIZ];

    if (args.size() != 1) {
        std::cerr << "No input file specified" << std::endl;
        return;
    }
    filename = args[0];
    std::cout << filename << std::endl;
    if (stat(filename.c_str(), &file_stat) != 0) {
        std::cerr << "File specified: " << filename << " doesn't exist" << std::endl;
        return;
    }
    
    // Send UP request to server
    std::cout << "send UP" << std::endl;
    strcpy(buffer, "UP");
    if ((send(sockfd, buffer, strlen(buffer), 0)) < 0) {
        perror("Failed to send UP request");
        return;
    }

    // Send filename length and filename
    std::cout << "send length" << std::endl;
    filename_len = htons((uint16_t)filename.length());
    if ((send(sockfd, &filename_len, sizeof(uint16_t), 0)) < 0) {
        perror("Failed to send filename length");
        return;
    }
    std::cout << "send filename" << std::endl;
    if ((send(sockfd, filename.c_str(), filename.length(), 0)) < 0) {
        perror("Failed to send filename");
        return;
    }
}
