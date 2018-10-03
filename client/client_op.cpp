/*
 * File: client_operation.cpp
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

#include "myftp.h"

void get_ls(int sockfd) {
    uint32_t size;
    std::string listing;

    // Send LS request to server
    if (send_string(sockfd, std::string("LS")) < 0) {
        std::cerr << "Client fails to send LS request" << std::endl;
        return;
    }

    // Recv listing length
    if (recv_string(sockfd, listing) < 0) {
        std::cerr << "Client fails to receive listing" << std::endl;
        return;
    }

    std::cout << listing << std::endl;
    return;
}

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

void make_directory(int sockfd, std::vector<std::string> args){
  std::string dir_name;
  std::string confirm;

  // Check if directory name was input
  if (args.size() != 1) {
      std::cerr << "No directory name specified" << std::endl;
      return;
  }

  dir_name = args[0];

  // Send MKDIR request to server
  if (send_string(sockfd, std::string("MKDIR")) < 0) {
      std::cerr << "ERROR send MKDIR request" << std::endl;
      return;
  }


  // Send directory name to server
  if (send_filename(sockfd, dir_name) < 0) {
      std::cerr << "Client fails to send directory name to server" << std::endl;
      return;
  }

  // Receive Confirm int
  if (recv_string(sockfd, confirm) < 0) {
      std::cerr << "Client fails to receive confirm" << std::endl;
  }

  switch (stoi(confirm)){
    case -2:
        std::cerr << "The directory already exists on server" << std::endl;
        break;
    case -1:
        std::cerr << "Error in making directory" << std::endl;
        break;
    case 1:
        std::cout << "The directory was successfully made" << std::endl;
        break;
  }

  return;
}

void remove_file(int sockfd, std::vector<std::string> args){
    struct stat file_stat;
    std::string filename;
    std::string md5sum;
    std::string ret_md5sum;
    std::string confirm;
    char userInput[BUFSIZ];

    // Check if directory name was input
    if (args.size() != 1) {
        std::cerr << "No file name specified" << std::endl;
        return;
    }

    filename = args[0];

    // Send RM request to server
    if (send_string(sockfd, std::string("RM")) < 0) {
        std::cerr << "ERROR send RM request" << std::endl;
        return;
    }

    // Send filename to server
    if (send_filename(sockfd, filename) < 0) {
        std::cerr << "Client failed sending filename to server" << std::endl;
        return;
    }

    // Receive Confirm int
    if (recv_string(sockfd, confirm) < 0) {
        std::cerr << "Client failed receiving confirm" << std::endl;
        return;
    }

    if(stoi(confirm) != 1){
        std::cerr << "The file does not exist on server" << std::endl;
        return;
    }

    std::cout << "Are you sure? (Yes/No): ";

    // Gets answer from user
    if (fgets(userInput, BUFSIZ, stdin) == NULL){
        std::cerr << "Client failed receiving user input" << std::endl;
        return;
    }

    // Checks user input and updates confirm
    if (strncmp(userInput, "No", strlen("No")) == 0){
        std::cout << "Delete abandoned by the user!" << std::endl;
        confirm = "No";
    }
    else if(strncmp(userInput, "Yes", strlen("Yes")) == 0){
        confirm = "Yes";
    }

    // Sends confirm to server
    if (send_string(sockfd, confirm) < 0) {
        std::cerr << "Client failed sending confirm" << std::endl;
        return;
    }


    // Receives confirm from server - if file got deleted
    if (recv_string(sockfd, confirm) < 0) {
        std::cerr << "Client fails to receive confirm" << std::endl;
        return;
    }

    if (confirm == "Yes"){
        std::cout << "File deleted" << std::endl;
    }

    return;
}

void remove_directory(int sockfd, std::vector<std::string> args) {
    std::string dir_name;
    std::string ack;
    std::string confirmation;
    std::string user_input;

    // send RMDIR request to server
    if (send_string(sockfd, std::string("RMDIR")) < 0) {
        std::cerr << "Client fails to send RMDIR request to server" << std::endl;
        return;
    }

    // Send directory name to server
    dir_name = args[0];
    if (send_filename(sockfd, dir_name) < 0) {
        std::cerr << "Client fails to send directory name to server" << std::endl;
        return;
    }

    // Receive confirmation from server
    if (recv_string(sockfd, confirmation) < 0) {
        std::cerr << "Client fails to receive confirmation from server" << std::endl;
        return;
    }

    // Directory doesn't exist
    if (!confirmation.compare("-1")) {
        std::cout << "The directory does not exist on server" << std::endl;
        return;
    // Directory is not empty
    } else if (!confirmation.compare("-2")){
        std::cout << "The directory is not empty" << std::endl;
        return;
    }

    // Ask user to confirm deletion
    std::cout << "Do you wish to delete " << dir_name << ". Yes to delete, No to ignore > ";
    std::cin >> user_input;

    if (send_string(sockfd, user_input) < 0) {
        std::cerr << "Client fails to send confirmation to server" << std::endl;
        return;
    }

    // User abandons rmdir
    if (!user_input.compare("NO")) {
        std::cout << "Delete abandoned by the user!" << std::endl;
        return;
    }

    // Receive rmdir ack
    if (recv_string(sockfd, ack) < 0) {
        std::cerr << "Client fails to receive delete ack" << std::endl;
        return;
    }

    if (!ack.compare("-1"))
        std::cout << "Failed to delete directory" << std::endl;
    else
        std::cout << "Directory deleted" << std::endl;

    return;
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

void change_directory(int sockfd, std::vector<std::string> args){
    std::string dir_name;
    std::string confirm;

    // Check if directory name was input
    if (args.size() != 1) {
        std::cerr << "No directory name specified" << std::endl;
        return;
    }

    dir_name = args[0];

    // Send MKDIR request to server
    if (send_string(sockfd, std::string("CD")) < 0) {
        std::cerr << "ERROR send CD request" << std::endl;
        return;
    }

    // Send directory name to server
    if (send_filename(sockfd, dir_name) < 0) {
        std::cerr << "Client fails to send directory name to server" << std::endl;
        return;
    }

    // Receive Confirm int
    if (recv_string(sockfd, confirm) < 0) {
        std::cerr << "Client fails to receive confirm" << std::endl;
    }

    switch (stoi(confirm)){
      case -2:
          std::cerr << "The directory does not exist on server" << std::endl;
          break;
      case -1:
          std::cerr << "Error in changing directory" << std::endl;
          break;
      case 1:
          std::cout << "Changed current directory" << std::endl;
          break;
    }

    return;
}
