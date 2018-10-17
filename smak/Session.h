//
// Created by michael on 10/12/18.
//

#ifndef SMAK_SESSION_H
#define SMAK_SESSION_H


#include <netinet/in.h>
#include <string>

class Session {
public:
    static int sessionCount;
    Session(struct sockaddr_in serv_addr, char buffer[], size_t bufferLength, int sockFd);
    int listenForAndPrint(std::string terminateMessage);

private:
    struct sockaddr_in socket;
    int socketFd;
    int instanceNum;
    char* buffer;
    size_t bufferLength;
};


#endif //SMAK_SESSION_H
