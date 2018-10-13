//
// Created by michael on 10/12/18.
//

#include <strings.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "Session.h"
#include "Server.h"

Session::Session(struct sockaddr_in socket, char *buffer, size_t bufferLength, int sockFd) {
    this->socket = socket;
    this->buffer = buffer;
    this->bufferLength = bufferLength;
    this->socketFd = sockFd;
    std::cout<<"Built session\n";
    bzero(buffer,256);
}

int Session::listenForAndPrint(std::string terminateMessage) {
    std::cout<<"Listening to port\n";
    ssize_t errNo;
    std::string inString;
    while (true){
        memset(buffer, 0, bufferLength);
        errNo = read(socketFd,buffer,bufferLength);
        if (errNo < 0)
            Server::error("ERROR reading from socket");
        inString = buffer;
        if (inString == terminateMessage){
            std::cout<<"I got: " << inString << "\n";
            errNo = write(socketFd,"GOODBYE",18);
            close(socketFd);
            return 0;
        }
        std::cout<<"I got: " << inString <<"\n";
        errNo = write(socketFd,"I got your message",18);
        if (errNo < 0)
            Server::error("ERROR writing to socket");
    }

    return 0;
}
