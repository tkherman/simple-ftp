/*
File: myftp.cpp
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1
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


int main(int argc, char* argv[])
{
    int sockfd;
    int port;
    char *host;
    char buf[BUFSIZ];
    bool running = true;

    if (argc != 3)
    {
        fprintf(stderr, "usage: wrong number of arguments\n");
        exit(1);
    }
    host = argv[1];
    port = std::stoi(argv[2]);

    if ((sockfd = socket_connect(host, port)) < 0) {
        exit(1);
    }

    
    while(running)
    {
        memset(&buf, 0, BUFSIZ);
        fputs("> ", stdout);
        fgets(buf, BUFSIZ, stdin);
        size_t len = strlen(buf);
        if (buf[len-1] == '\n') {
            buf[len-1] = '\0';
        }

        if (!strcmp(buf, "EXIT")) {
            if ((send(sockfd, buf, BUFSIZ, 0)) < 0) {
                fprintf(stderr, "Failed to send message");
                continue;
            }
            running = false;
            printf("Closing connection");
        } else {
            if ((send(sockfd, buf, BUFSIZ, 0)) < 0) {
                fprintf(stderr, "Failed to send message");
                continue;
            }
        }
    }

    close(sockfd);
}




