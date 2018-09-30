/*
 * File: server_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 *
 */

#include "myftpd.h"

void receive_upload_file(int sockfd) {
    std::cout << "Handling file upload from client" << std::endl;
    std::string filename;
    uint32_t file_size;
    
    // Get filename from client
    if(recv_filename(sockfd, filename) < 0) {
        std::cerr << "Server failed to get filename from client" << std::endl;
        return;
    }

    std::cout << "Client attempt to upload: " << filename << std::endl;

    // Resend filename back to client to acknowledge client that server
    // is ready
    if (send_filename(sockfd, filename) < 0) {
        std::cerr << "Failed to resend filename back to client" << std::endl;
        return;
    }

    // Receive file size from client
    if (recv_file_size(sockfd, file_size) < 0) {
        std::cerr << "Server fails to receive file size from client" << std::endl;
        return;
    }

    recv_file(sockfd, file_size, filename);

    return;
}
