//
// Created by michael on 10/4/18.
//

#include "client.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>

int client::clientMain(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    ssize_t errNo;

    int portNo = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    struct hostent *server;
    server = gethostbyname(argv[1]);
    if (server == nullptr) {
        fprintf(stderr,"ERROR, no such host\n");
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
    // now we'll let you enter messages
//    char buffer[256];
//    uint bufferSize = 256;
//    while (true){
//        memset(buffer, 0, bufferSize);
//        printf("Please enter the message: ");
//        fgets(buffer,255,stdin);
//        errNo = write(sockfd,buffer,strlen(buffer));
//        if (errNo < 0)
//            error("ERROR writing to socket");
//        memset(buffer, 0, 256);
//        errNo = read(sockfd,buffer,255);
//        if (errNo < 0)
//            error("ERROR reading from socket");
//
//        printf("%s\n",buffer);
//        std::string recString = buffer;
//        if (recString == "GOODBYE"){
//            close(sockfd);
//            return 0;
//        }
//    }

    std::thread listener = std::thread(client::listenAndPrint, sockfd);
    std::thread writer = std::thread(client::writeSock, sockfd);
    listener.join();
    writer.join();


}

void client::listenAndPrint(int sockFd) {
    char buffer[256];
    uint bufferSize = 256;
    ssize_t errNo;
    while(true){
        memset(buffer, 0, 256);
        errNo = read(sockFd,buffer,255);
        if (errNo < 0)
            error("ERROR reading from socket");

        printf("%s\n",buffer);
        std::string recString = buffer;
        if (recString == "GOODBYE"){
            close(sockFd);
            return;
        }
    }
}

void client::writeSock(int sockFd) {
    char buffer[256];
    uint bufferSize = 256;
    ssize_t errNo;
    while(true){
        memset(buffer, 0, bufferSize);
        printf("Please enter the message: ");
        fgets(buffer,255,stdin);
        errNo = write(sockFd,buffer,strlen(buffer));
        if (errNo < 0)
            error("ERROR writing to socket");
    }
}
