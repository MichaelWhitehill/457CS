//
// Created by michael on 11/6/18.
//

#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "smakerClient.h"
#include "../smak/main.h"

smakerClient::smakerClient(std::string hostname, int portNo) {
    ssize_t errNo;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR: opening socket");
    struct hostent *server;
    server = gethostbyname(hostname.c_str());

    if (server == nullptr) { //also doubles as a check for the clientState struct var
        fprintf(stderr,"ERROR: no such host");
        exit(0);
    }
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(static_cast<uint16_t>(portNo));
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
}

void smakerClient::closeCon() {
    close(sockfd);
}

std::vector<std::string> smakerClient::sendGetResponces(std::string jsonStr, int responseCount) {
    sendStr(jsonStr);
    return listenForMessages(responseCount);
}

void smakerClient::sendStr(std::string toSend) {
    ssize_t errNo = write(sockfd, toSend.c_str(), toSend.size());
    if(errNo < 0){
        throw std::string("Test error. Failed to send string: " + toSend);
    }
}

std::string smakerClient::getSrvResponse() {
    char buffer[256];
    uint bufferSize = 256;
    ssize_t errNo;
    memset(buffer, 0, 256);
    errNo = read(sockfd,buffer,255);
    if (errNo < 0)
        throw std::string("Test error. Failed to read string from server.");

    std::string recString = buffer;
    return recString;
}

std::vector<std::string> smakerClient::listenForMessages(uint messageCount) {
    std::vector<std::string> responses;
    for (uint i = 0; i < messageCount; ++i){
        std::string res = getSrvResponse();
        responses.push_back(res);
    }
    return responses;
}
