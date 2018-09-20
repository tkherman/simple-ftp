/*
File: myftp.cpp
Name: Josefa Osorio, Herman Tong, Jessica Hardey
Netid: josorio2, ktong1
*/

int main(int argc, char* argv[])
{
    char *host;
    int PORT;
    struct hostent *hp;
    struct sockaddr_in sin;
    int s;
    char buf[4096];

    if (argc == 3)
    {
        host = argv[1];
        PORT = std::stoi(argv[2]);
    }
    else
    {
        fprintf(stderr, "usage: wrong number of arguments\n");
        exit(1);
    }

    hp = gethostbyname(host);
    if (!hp)
    {
        fprintf(stderr, "unknown host: %s\n", host);
        exit(1);
    }

    // Build address data structure
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(PORT);

    // active open
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error\n");
        exit(1);
    }

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("connection error\n");
        close(s);
        exit(1);
    }

    while(fgets(buf, sizeof(buf), stdin))
    {
        if (send(s, buf, sizeof(buf), 0) == -1)
        {
            perror("send error\n");
            exit(1);
        }
    }

    close(s);
}
