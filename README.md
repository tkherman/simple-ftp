Name: Herman Tong, Josefa Osorio, Jessica Hardey
NetId: ktong1, josorio2, jhardey

All files:
    Makefile
    REAdME.md
    client/
        client_op.cpp
        myftp.cpp
        myftp.h
    network_utils/
        network_utils.cpp
        network_utils.h
    server/
        myftpd.cpp
        myftpd.h
        server_op.cpp

To run the program, run make in the simple-ftp directory.
Then to run the client, go to client/bin and run the ./myftp executable
To run the server, go to server/bin and run the ./myftpd executable

Don't run the executables in the same directory, as UP and DL would fail since
the programs will be reading and writing to the same file at the same time.
