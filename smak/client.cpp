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
#include <iostream>
#include<getopt.h>
#include<fstream>

struct clientInfo{
    char *hostName = nullptr;
    std::string userName, configFile, testFile; //config File & test File parameter will be string to existing file
    int port = 0;    //destination server port
    std::ofstream logFile;   //string logFile name
}clientState;




int client::clientMain(int argc, char *argv[])
{

    if (argc < 2) {
        std::cerr<< "Incorrect usage: not enough arguments, Client minimum arguments: -c 'configFileName.conf' (in current directory) clt";
        exit(0);
    }std::cout <<"[Starting Client]" << std::endl;


    parseArgs(argc, argv);


   if(!clientState.configFile.empty()){
       std::cout << "[Initializing Client from .config file]"<<std::endl;
       //Config file has been supplied as single arg and vars will be populated from file:
        //TODO: parse config file and cast string hostname to *char?
//outfile << "my text here" << std:: endl;
//outfile.close()

   }


    ssize_t errNo;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    struct hostent *server;
    server = gethostbyname(clientState.hostName);

    if (server == nullptr) { //also doubles as a check for the clientState struct var
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }


    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(static_cast<uint16_t>(clientState.port));
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    //TODO: Make clean exit where the sockets get closed
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
    // TODO: Add exit condition and gracefully close
    while(true){
        memset(buffer, 0, bufferSize);
        printf("Please enter the message: ");
        fgets(buffer,255,stdin);
        errNo = write(sockFd,buffer,strlen(buffer));
        if (errNo < 0)
            error("ERROR writing to socket");
    }
}

void client::parseArgs(int argc, char *argv[]){
    //processing arguments with getOpt:

    //Run switch statement below regardless of # of args provided so that configFile is populated in either case:

    int option;
    while ((option = getopt(argc, argv, "h:u:p:c:t:L:")) != -1) {
        switch (option) {
            case 'h' :
                clientState.hostName = optarg;
                std::cout << "Client hostName: " << clientState.hostName << std::endl;
                break;
            case 'u' :
                clientState.userName = optarg;
                std::cout << "Client userName: " << clientState.userName << std::endl;
                break;
            case 'p' :
                clientState.port = atoi(optarg);
                std::cout << "Client Port: " << clientState.port << std::endl;
                break;
            case 'c' :
                clientState.configFile = optarg;
                std::cout << "Config File name: " << clientState.configFile << std::endl;
                break;
            case 't' :
                clientState.testFile = optarg;
                std::cout << "Test File name: " << clientState.testFile << std::endl;
                break;
            case 'L' : {
                std::ofstream logFile = std::ofstream(optarg);
                std::cout << "Log File has been opened under name: " << optarg << std::endl;
                break;
            }
            default  :
                std::cerr
                        << "Incorrect usage: Incorrect arguments, Client args are in the form: -h hostname -u username -p serverport ";
                exit(0);
        }
    }

}