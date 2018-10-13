//
// Created by michael on 10/2/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "Server.h"
#include "Session.h"


int Server::serverMain (int argc, char** argv){
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    // Initialize the portnumber to arg[1]
    int portNo = atoi(argv[1]);
    int sockFd, newSockFd;
    socklen_t cliLength;
    char buffer[256];

    // create a sockaddr struct
    struct sockaddr_in serv_addr, cli_addr;
    ssize_t errNo;
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
        Server::error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNo);
    if (bind(sockFd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        Server::error("ERROR on binding");
    listen(sockFd,5);
    cliLength = sizeof(cli_addr);
    newSockFd = accept(sockFd,
                       (struct sockaddr *) &cli_addr,
                       &cliLength);
    if (newSockFd < 0)
        Server::error("ERROR on accept");
    std::cout << "Accepted connection\n";

    // Make session here
    Session ses = Session(serv_addr, buffer, 255, newSockFd);
    ses.listenForAndPrint("EXIT\n");
    close(sockFd);
    return 0;
}

void Server::error(const char *msg) {
    perror(msg);
    exit(1);
}
