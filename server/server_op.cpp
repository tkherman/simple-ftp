/*
 * File: server_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey1
 *
 */

#include "myftpd.h"

std::string get_dir_listing() {
    std::string listing;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    std::string cmd("ls -l");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while(!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL)
                listing.append(buffer);
        }
        pclose(stream);
    }

    return listing;
}

void service_ls_request(int sockfd) {
    std::string listing;

    // Get and send listing
    listing = get_dir_listing();
    if (send_string(sockfd, listing) < 0) {
        std::cerr << "Server fails to send lising" << std::endl;
        return;
    }

    return;
}

void receive_upload_file(int sockfd) {
    std::string filename;
    std::string md5sum;
    uint32_t throughput;
    uint32_t file_size;
    struct timeval st, et;
    int time_elasped;
    
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

    // Receive file and calculate throughput
    gettimeofday(&st, NULL);
    if (recv_file(sockfd, file_size, filename) < 0) {
        std::cerr << "Server fails to receive file from client" << std::endl;
        return;
    }
    gettimeofday(&et, NULL);
    time_elasped = get_time_elasped(st, et);
    throughput = (file_size * 1000000) / time_elasped;
    
    std::cout << "Received file from client: " << filename << std::endl;

    // Calculate md5sum, then send md5 sum and throughput to client
    md5sum = get_file_md5(filename);
    std::cout << md5sum << std::endl;
    if (send_string(sockfd, md5sum) < 0) {
        std::cerr << "Server failed to send md5sum to client" << std::endl;
        return;
    }
    if (send_file_size(sockfd, throughput) < 0) { // send_file_size() sends 32-bit int, can rename
        std::cerr << "Server failed to send throughput to client" << std::endl;
        return;
    }

    return;
}
