/*
 * File: server_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 *
 */

#include "myftpd.h"

void receive_upload_file(int sockfd, char* buf) {
    std::cout << "handling recving file" << std::endl;
    int n;
    uint16_t filename_len;
    std::string filename;
    //char buf[BUFSIZ];
    
    // Receive filename length and filename
    memset(&buf, 0, BUFSIZ);
    if ((n = recv(sockfd, buf, BUFSIZ-1, 0)) < 0) {
        perror("Failed to receive filename length");
        return;
    }
    std::cout << buf << std::endl;
    memcpy((void*)&filename_len, (void*)buf, n);
    filename_len = ntohs(filename_len);

    memset(&buf, 0, BUFSIZ);
    if ((recv(sockfd, buf, BUFSIZ-1, 0)) < 0) {
        perror("Failed to receive filename");
        return;
    }
    filename = std::string(buf);
    
    std::cout << "filename length: " << filename_len << std::endl;
    std::cout << "filename       : " << buf << std::endl;
    std::flush(std::cout);

    return;
}
