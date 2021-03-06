/*
 * File: server_op.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
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
    
    // Turn filepath into filename
    filename = filename.substr(filename.find_last_of('/')+1, filename.size()-filename.find_last_of('/')+1);

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

void service_mkdir_request(int sockfd){
    struct stat dir_stat;
    std::string dir_name;
    std::string str;
    uint32_t file_size;
    const char *command;

    // Get directory name from client
    if(recv_filename(sockfd, dir_name) < 0) {
        std::cerr << "Server failed to get directory name from client" << std::endl;
        return;
    }


    // Checks if directory exists
    if (stat(dir_name.c_str(), &dir_stat) == 0 && (dir_stat.st_mode & S_IFDIR)){
        // Send client negative confirm integer - directory already exists
        if (send_string(sockfd, std::string("-2")) < 0) {
            std::cerr << "Server failed sending negative confirm" << std::endl;
            return;
        }
        return;
    }

    str = "mkdir " + dir_name;
    command = str.c_str();
    // Runs command
    if (system(command) < 0){
        // Sends client negative confirm integer - unable to make directory
        if (send_string(sockfd, std::string("-1")) < 0) {
            std::cerr << "Server failed sending negative confirm" << std::endl;
            return;
        }
        return;
    }

    // Sends client positive confirm integer
    if (send_string(sockfd, std::string("1")) < 0) {
        std::cerr << "Server failed sending positive confirm" << std::endl;
        return;
    }

    return;
}

void service_rm_request(int sockfd){
    struct stat file_stat;
    std::string filename;
    std::string str;
    uint32_t file_size;
    const char *command;
    std::string confirm;

    // Get filename from client
    if(recv_filename(sockfd, filename) < 0) {
        std::cerr << "Server failed to get filename from client" << std::endl;
        return;
    }

    if (stat(filename.c_str(), &file_stat) != 0)
        std::cout << "file doesn't exist" << std::endl;
    else if (file_stat.st_mode & S_IFDIR)
        std::cout << "it's a dir" << std::endl;

    // Checks if file exists and isn't a directory
    if (stat(filename.c_str(), &file_stat) != 0  || (file_stat.st_mode & S_IFDIR)) {
        // Send client negative confirm integer - file doesn't exist
        if (send_string(sockfd, std::string("-1")) < 0) {
            std::cerr << "Server failed sending positive confirm" << std::endl;
            return;
        }
        return;
    }

    if (send_string(sockfd, std::string("1")) < 0) {
        std::cerr << "Server failed sending positive confirm" << std::endl;
        return;
    }

    if (recv_string(sockfd, confirm) < 0) {
        std::cerr << "Server failed receiving confirm" << std::endl;
        return;
    }

    // Checks confirm and sends to client if No
    if (confirm == "No"){
        if (send_string(sockfd, std::string("No")) < 0) {
            std::cerr << "Server failed sending negative confirm" << std::endl;
            return;
        }
        return;
    }

    // Removes file if confirm is Yes
    str = "rm " + filename;
    command = str.c_str();
    // Runs command
    if (system(command) < 0){
        // Sends client negative confirm integer - unable to make directory
        if (send_string(sockfd, std::string("No")) < 0) {
            std::cerr << "Server failed sending negative confirm" << std::endl;
            return;
        }
        return;
    }

    // Sends positive confirm to client
    if (send_string(sockfd, std::string("Yes")) < 0) {
        std::cerr << "Server failed sending positive confirm" << std::endl;
        return;
    }

    return;
}

void service_rmdir_request(int sockfd) {
    std::string dir_name;
    std::string confirmation;
    int dir_status;
    struct stat info;

    // Receive directory name from client
    if (recv_filename(sockfd, dir_name) < 0) {
        std::cerr << "Server fails to receive directory name from client" << std::endl;
        return;
    }

    // Check if directory exist and if it's empty
    dir_status = get_dir_info(dir_name);
    if (send_string(sockfd, std::to_string(dir_status)) < 0) {
        std::cerr << "Server fails to send directory status to client" << std::endl;
        return;
    }

    // End operation if directory cannot be deleted
    if (dir_status != 1)
        return;

    // Get delete confirmation
    if (recv_string(sockfd, confirmation) < 0) {
        std::cerr << "Server fails to receive delete confirmation" << std::endl;
        return;
    }

    if (!confirmation.compare("No"))
        return;

    if (send_string(sockfd, std::to_string(rmdir(dir_name.c_str()))) < 0) {
        std::cerr << "Server fails to send delete ack" << std::endl;
        return;
    }

    return;
}

void send_download_file(int sockfd) {
    struct stat file_stat;
    std::string filename;
    std::string md5sum;
    uint32_t throughput;
    uint32_t file_size;

    // Get filename from client
    if(recv_filename(sockfd, filename) < 0) {
        std::cerr << "Server failed to get filename from client" << std::endl;
        return;
    }

    // check if file exists
    if (stat(filename.c_str(), &file_stat) != 0) {
        if (send_file_size(sockfd, (uint32_t)-1) < 0) {
            std::cerr << "Failed to send -1 to client" << std::endl;
            return;
        }
        std::cerr << "File specified: " << filename << " doesn't exist" << std::endl;
        return;
    }

    // Send file size
    std::cout << "file_size: " << (uint32_t)file_stat.st_size << std::endl;
    if (send_file_size(sockfd, (uint32_t)file_stat.st_size) < 0) {
        std::cerr << "Server fails to send file size to client" << std::endl;
        return;
    }

    md5sum = get_file_md5(filename);
    std::cout << "md5sum: " << md5sum << std::endl;
    if (send_string(sockfd, md5sum) < 0) {
        std::cerr << "Server failed to send md5sum to client" << std::endl;
        return;
    }

    // Send file
    if (send_file(sockfd, filename) < 0) {
        std::cerr << "Server fails to send file to server" << std::endl;
        return;
    }

    return;
}

void service_cd_request(int sockfd){
    struct stat dir_stat;
    std::string dir_name;
    uint32_t file_size;
    std::string curr_path;
    std::string changed_path;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    std::string cmd("pwd");

    // Get directory name from client
    if(recv_filename(sockfd, dir_name) < 0) {
        std::cerr << "Server failed to get directory name from client" << std::endl;
        return;
    }

    // Checks if directory exists
    if (stat(dir_name.c_str(), &dir_stat) != 0 || !(dir_stat.st_mode & S_IFDIR)){
        // Send client negative confirm integer - directory doesn't exist
        if (send_string(sockfd, std::string("-2")) < 0) {
            std::cerr << "Server failed sending negative confirm" << std::endl;
            return;
        }
        return;
    }

    // Gets directory before switching
    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while(!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL)
                curr_path.append(buffer);
        }
        pclose(stream);
    }

    if (chdir(dir_name.c_str()) < 0){
        std::cerr << "Server failed changing directory" << std::endl;
        return;
    }

    // Gets directory after changing directory
    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while(!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL)
                changed_path.append(buffer);
        }
        pclose(stream);
    }

    // Checks if there was an error changing directories
    if (curr_path.compare(changed_path) == 0){
      // Send client negative confirm integer - error changing directory
      if (send_string(sockfd, std::string("-1")) < 0) {
          std::cerr << "Server failed sending negative confirm" << std::endl;
          return;
      }
    }


    // Send client positive confirm integer - directory changed
    if (send_string(sockfd, std::string("1")) < 0) {
        std::cerr << "Server failed sending positive confirm" << std::endl;
        return;
    }

    return;
}
