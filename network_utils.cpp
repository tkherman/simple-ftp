/*
 * File: network_utils.cpp
 * Name: Herman Tong, Jessica Hardey, Josefa Osorio
 * Netid: ktong1, jhardey1, josorio2
 */

#include "network_utils.h"

int send_string(int sockfd, std::string msg) {
    int ret;
    size_t len;

    // send length of msg
    len = msg.length();
    if ((ret = send(sockfd, &len, sizeof(size_t), 0)) < 0) {
        perror("ERROR sending string length");
        return ret;
    }

    // send msg
    if ((ret = send(sockfd, msg.c_str(), len, 0)) < 0) {
        perror ("ERROR sending string message");
        return ret;
    }

    return ret;
}

int recv_string(int sockfd, std::string &msg) {
    int ret;
    size_t len;
    char buffer[BUFSIZ];

    // recv length of msg
    memset(buffer, 0, BUFSIZ);
    if ((ret = recv(sockfd, buffer, sizeof(size_t), 0)) < 0) {
        perror("ERROR recving string length");
        return ret;
    }
    memcpy(&len, buffer, sizeof(size_t));

    if ((ret = recv(sockfd, buffer, len, 0)) < 0) {
        perror("ERROR recving string message");
        return ret;
    }
    msg = std::string(buffer);
    return ret;
}

int send_filename(int sockfd, std::string filename) {
    int ret;
    uint16_t filename_len;

    // send filename length
    filename_len = htons((uint16_t)filename.size());
    if ((ret = send(sockfd, &filename_len, sizeof(uint16_t), 0)) < 0) {
        perror("ERROR sending filename length");
        return ret;
    }

    // send filename
    if ((ret = send(sockfd, filename.c_str(), filename_len, 0)) < 0) {
        perror("ERROR sending filename");
        return ret;
    }

    return ret;
}

int recv_filename(int sockfd, std::string &filename) {
    int ret;
    uint16_t filename_len;
    char buffer[BUFSIZ];

    // recv filename length
    memset(buffer, 0, BUFSIZ);
    if ((ret = recv(sockfd, buffer, sizeof(uint16_t), 0)) < 0) {
        perror("ERROR recving filename length");
        return ret;
    }
    memcpy(&filename_len, buffer, sizeof(uint16_t));
    
    // recv filename
    if ((ret = recv(sockfd, buffer, (size_t)filename_len, 0)) < 0) {
        perror("ERROR recving filename");
        return ret;
    }
    filename = std::string(buffer);

    return ret;
}

int send_file_size(int sockfd, uint32_t size) {
    int ret;
    
    // send 32-bit file size
    size = htonl(size);
    if ((ret = send(sockfd, &size, sizeof(uint32_t), 0)) < 0)
        perror("ERROR sending 32-bit file size");

    return ret;
}

int recv_file_size(int sockfd, uint32_t &size) {
    int ret;
    uint32_t temp_size;
    char buffer[BUFSIZ];

    // recv 32-bit file size
    memset(buffer, 0, BUFSIZ);
    if ((ret = recv(sockfd, buffer, sizeof(uint32_t), 0)) < 0) {
        perror("ERROR receiving 32-bit file size");
        return ret;
    }
    memcpy(&temp_size, buffer, sizeof(uint32_t));
    size = ntohl(temp_size);

    return ret;
}

// https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/
std::string get_file_md5(std::string filename) {
    std::string md5sum;
    std::string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    std::string cmd("md5sum ");
    cmd += filename;

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while(!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL)
                data.append(buffer);
        }
        pclose(stream);
    }

    // parse for md5sum
    std::istringstream ss(data);
    std::getline(ss, md5sum, ' ');

    return md5sum;
}

int send_file(int sockfd, std::string filename) {
    int ret;
    char buffer[BUFSIZ];
    std::ifstream file;
    file.open(filename, std::ios::binary);

    if (file.is_open()) {
        while (!file.eof()) {
            // read from file
            memset(buffer, 0, BUFSIZ);
            file.read(buffer, BUFSIZ);
            
            // send binary data over socket
            if ((ret = send(sockfd, buffer, file.gcount(), 0)) < 0) {
                perror("ERROR sending file");
                break;
            }
        }
    }

    file.close();
}

int recv_file(int sockfd, uint32_t size, std::string filename) {
    int ret;
    uint32_t received = 0;
    char buffer[BUFSIZ];
    std::ofstream file;
    file.open(filename, std::ios::binary | std::ios::trunc);

    if (file.is_open()) {
        while (received < size) {
            // receive binary data over socket
            memset(buffer, 0, BUFSIZ);
            if ((ret = recv(sockfd, buffer, BUFSIZ, 0)) < 0) {
                perror("ERROR receiving file");
                break;
            }

            // write to file
            file.write(buffer, ret);
            received += ret;
        }
    }

    file.close();
}

int get_time_elasped(struct timeval st, struct timeval et) {
    return (((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec));
}



















