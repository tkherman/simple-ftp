/*
File: myftp.cpp
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1, jhardey
*/

#include "myftp.h"

int socket_connect(char *host, int port) {
    int s;
    struct hostent *hp;
    struct sockaddr_in sin;
    hp = gethostbyname(host);
    if (!hp)
    {
        fprintf(stderr, "unknown host: %s\n", host);
        return -1;
    }

    // Build address data structure
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(port);

    // active open
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error\n");
        return -1;
    }

    printf("Connecting to %s\n", host);
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("connection error\n");
        close(s);
        return -1;
    }
    printf("Connection established\n");

    return s;
}

Operation parse_input(std::vector<std::string>& arguments) {
    std::string input;
    std::cout << "> ";
    getline(std::cin, input);

    Operation op;
    std::string token;
    std::istringstream ss(input);

    std::getline(ss, token, ' ');
    if (!token.compare("DL"))
        op = DL;
    else if (!token.compare("UP"))
        op = UP;
    else if (!token.compare("RM"))
        op = RM;
    else if (!token.compare("LS"))
        op = LS;
    else if (!token.compare("MKDIR"))
        op = MKDIR;
    else if (!token.compare("RMDIR"))
        op = RMDIR;
    else if (!token.compare("CD"))
        op = CD;
    else if (!token.compare("EXIT"))
        op = EXIT;
    else
        op = UNKNOWN;

    while (std::getline(ss, token, ' ')) {
        arguments.push_back(token);
    }

    return op;
}


int main(int argc, char* argv[])
{
    int sockfd;
    int port;
    char *host;
    char buf[BUFSIZ];
    bool running = true;
    Operation op;
    std::vector<std::string> command_arguments;

    if (argc != 3)
    {
        std::cerr << "usage: wrong number of arguments" << std::endl;
        exit(1);
    }
    host = argv[1];
    port = std::stoi(argv[2]);

    if ((sockfd = socket_connect(host, port)) < 0) {
        exit(1);
    }

    while (running) {
        command_arguments = std::vector<std::string>();
        op = parse_input(command_arguments);

        switch(op) {
            case DL:
                download_file(sockfd, command_arguments);
                break;
            case UP:
                upload_file(sockfd, command_arguments);
                break;
            case RM:
                remove_file(sockfd, command_arguments);
                break;
            case LS:
                get_ls(sockfd);
                break;
            case MKDIR:
                make_directory(sockfd, command_arguments);
                break;
            case RMDIR:
                break;
            case CD:
                change_directory(sockfd, command_arguments);
                break;
            case EXIT:
                if (send_string(sockfd, std::string("EXIT")) < 0)
                    continue;
                running = false;
                std::cout << "Closing connection" << std::endl;
                break;
        }
    }

    close(sockfd);
}
