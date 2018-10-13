//
// Created by michael on 10/12/18.
//

#include <strings.h>
#include <unistd.h>
#include "Session.h"
#include "Server.h"

Session::Session(struct sockaddr_in socket, char *buffer, size_t bufferLength) {
    this->socket = socket;
    this->buffer = buffer;
    this->bufferLength = bufferLength;
    bzero(buffer,256);
}

int Session::listenForAndPrint(std::string terminateMessage) {
    ssize_t errNo;
    int sockFd = 0;
    int newSockFd = 0;
    errNo = read(newSockFd,buffer,255);
    if (errNo < 0) Server::error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
    errNo = write(newSockFd,"I got your message",18);
    if (errNo < 0) Server::error("ERROR writing to socket");
    return 0;
}
