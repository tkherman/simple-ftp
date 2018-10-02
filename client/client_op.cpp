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
    std::string md5sum;
    std::string ret_md5sum;
    uint32_t throughput;
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

    // Get md5sum
    md5sum = get_file_md5(filename);

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

    // Send file
    if (send_file(sockfd, filename) < 0) {
        std::cerr << "Client fails to send file to server" << std::endl;
        return;
    }

    // Recv checksum
    if (recv_string(sockfd, ret_md5sum) < 0) {
        std::cerr << "Client fails to receive md5checksum" << std::endl;
    }


    // Recv throughput
    if (recv_file_size(sockfd, throughput) < 0) {
        std::cerr << "Client fails to receive throughput" << std::endl;
    }

    if (md5sum.compare(ret_md5sum)) {
        std::cout << "Upload fails: md5sum returned is different, upload failed" << std::endl;
    } else {
        std::cout << "Upload successful" << std::endl;
        std::cout << "Throughput: " << throughput << " bytes/second" << std::endl;
        std::cout << "Md5sum: " << md5sum << std::endl;
    }
}

void download_file(int sockfd, std::vector<std::string> args) {
    std::string filename;
    std::string ret_filename;
    std::string md5sum;
    std::string ret_md5sum;
    struct stat file_stat;
    uint32_t throughput;
    uint32_t file_size;
    char buffer[BUFSIZ];
    struct timeval st, et;
    int time_elasped;

    // Check if input file exists
    if (args.size() != 1) {
        std::cerr << "No input file specified" << std::endl;
        return;
    }
    filename = args[0];

    // Send DL request to server
    if (send_string(sockfd, std::string("DL")) < 0) {
        std::cerr << "ERROR send DL request" << std::endl;
        return;
    }

    // Send filename to server
    if (send_filename(sockfd, filename) < 0) {
        std::cerr << "Client fails to send filename to server" << std::endl;
        return;
    }

    // Send file size
    if (send_file_size(sockfd, (uint32_t)file_stat.st_size) < 0) {
        std::cerr << "Client fails to send file size to server" << std::endl;
        return;
    }

    // Recv checksum
    if (recv_string(sockfd, ret_md5sum) < 0) {
        std::cerr << "Client fails to receive md5checksum" << std::endl;
    }

    // recv file size from server
    if (recv_file_size(sockfd, file_size) < 0) {
        std::cerr << "Client fails to receive file size from server" << std::endl;
        return;
    }

    // recv file from server
    gettimeofday(&st, NULL);
    if (recv_file(sockfd, file_size, filename) < 0) {
        std::cerr << "Client fails to receive file from server" << std::endl;
        return;
    }
    gettimeofday(&et, NULL);
    time_elasped = get_time_elasped(st, et);
    throughput = (file_size * 1000000) / time_elasped;


    // calculate hash and compare to hash from server
    md5sum = get_file_md5(filename);
    if (md5sum.compare(ret_md5sum)) {
        std::cout << "Upload fails: md5sum returned is different, download failed" << std::endl;
    } else {
        std::cout << "Download successful" << std::endl;
        std::cout << "Throughput: " << throughput << " bytes/second" << std::endl;
        std::cout << "Md5sum: " << md5sum << std::endl;
    }

}
