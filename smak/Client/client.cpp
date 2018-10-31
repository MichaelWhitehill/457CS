//
// Created by michael on 10/4/18.
//

#include "client.h"
#include "../main.h"
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
#include <vector>
#include <sstream>
#include <iterator>
#include "time.h"
#include <algorithm>
#include "makeMessage.h"


struct clientInfo{
    char *hostName = nullptr;
    std::string userName, configFile, testFile, logFile, Away; //config File & test File parameter will be string to existing file
    int port = 0;    //destination server port
    std::ofstream logFileWrite;   //string logFile name - file writer will be used by many function calls
}clientState;


static auto t = time(NULL); //var for current time


int client::clientMain(int argc, char *argv[])
{


    std::cout<<makeMessage::INVITE("Jonathan", "channel 1")<<std::endl;


    if (argc < 2) {
        std::cerr<< "Incorrect usage: not enough arguments, Client minimum arguments: -c 'configFileName.conf' (in current directory) clt";
        exit(0);
    }std::cout <<"[Starting Client]" << std::endl;


    parseArgs(argc, argv); //Parsing manually entered args to populate config file path by default


   if(fileExists(clientState.configFile)&&!clientState.configFile.empty()){
       //according to the TA in lab 10/18 we need to create a config file and populate it with the manual entries if no config file is provided
       //file will be formatted in the same way as manually passed args - SHOULD HAVE 5 LINES VS 6- no line for config file, as we already have that:
       //1 -h hostname
       //2 -u username
       //3 -p port#
       //4 -t text file path
       //5 -L log file path

       std::cout << "[Initializing Client from .config file]"<<std::endl;

       std::ifstream read = std::ifstream(clientState.configFile);

       if(!read.is_open()){
                error("ERROR: cannot open config File");
             exit(1);}

       for(std::string line; std::getline(read, line);){
        //parse the file line by line and populate vars vs messing with pointers
        std::vector<std::string> parse = split(line);
        if(parse[0]=="-h"){
            std::vector<char> temp(parse[1].length()+1);
            strcpy(&temp[0], parse[1].c_str());
            char* pc = new char[parse[1].length()+1];
            clientState.hostName= strcpy(pc, &temp[0]); //TODO: remove redundant steps above?
        }

        else if(parse[0]=="-u"){clientState.userName = parse[1];}
        else if(parse[0]=="-p"){clientState.port = stoi(parse[1]);}
        else if(parse[0]=="-t"){clientState.testFile = parse[1];}
        else if(parse[0]=="-L"){clientState.logFile = parse[1];}
        else error("ERROR: Incorrect formatting in .config file");

       }
       std::string uname = "[LOG]_";
       uname.append(clientState.userName);
       clientState.logFile.append(uname);

//       std::cout<<"\n[Client Variables set by .config file:]"<<std::endl
//       << "Client hostName: " << clientState.hostName << std::endl
//       << "Client userName: " << clientState.userName << std::endl
//       << "Client Port: " << clientState.port << std::endl
//       << "Config File name: " << clientState.configFile << std::endl
//       << "Test File name: " << clientState.testFile <<std::endl
//       << "Log File name: " << clientState.logFile << "\n" <<std::endl;
   }

   //create log file and set member var if file does not exist already: - **LogFile variable should include directory path!!
   if(!fileExists(clientState.logFile)){
       clientState.logFileWrite = std::ofstream(clientState.logFile);
       if(!clientState.logFileWrite.is_open()){error("ERROR: Unable to open log File for client"); exit(1);}
       else        std::cout<< "New Log File opened for writing\n" << std::endl;

       clientState.logFileWrite << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
               <<"Client hostName: " << clientState.hostName << std::endl
               << "Client Port: " << clientState.port << std::endl;


   }else {

       const char* remv = clientState.logFile.c_str();
       if(std::remove(remv)!=0){error("ERROR deleting existing logFile for client");}
       else {
           std::cout << "Old client log File successfully deleted - opening fresh file" << std::endl;
           clientState.logFileWrite = std::ofstream(clientState.logFile);
           std::cout<< "New Client Log File opened for writing" << std::endl;

           clientState.logFileWrite << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                   <<"Client hostName: " << clientState.hostName << std::endl
                   << "Client Port: " << clientState.port << std::endl;
       }
   }



   //FILES HAVE BEEN READ AND OPENED - SETUP CONNECTION WITH SERVER:
    ssize_t errNo;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR: opening socket");
    struct hostent *server;
    server = gethostbyname(clientState.hostName);

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
    serv_addr.sin_port = htons(static_cast<uint16_t>(clientState.port));
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    int disconnect = 0;
    std::thread listener = std::thread(client::listenAndPrint, sockfd, &disconnect);
    std::thread writer = std::thread(client::writeSock, sockfd, &disconnect);
    listener.join();
    writer.join();
    close(sockfd);

    //Closing logFile writer:
    clientState.logFileWrite.close();


}





void client::listenAndPrint(int sockFd, int* disconnect) {
    char buffer[256];
    uint bufferSize = 256;
    ssize_t errNo;
    while(true){
        memset(buffer, 0, 256);
        errNo = read(sockFd,buffer,255);
        if (errNo < 0)
            error("ERROR reading from socket");



        std::cout<< "RECIEVED: "<<buffer<<std::endl;
        std::string recString = buffer;

        //TODO: create switch statement:



        //std::endl automatically adds a \n and flushes the stream!
        if (recString == "GOODBYE"){
            *disconnect = 1;
            clientState.logFileWrite << "[" << getTime() << "] RECV: " <<recString << "EXITING CLIENT" << std::endl;
            if(clientState.logFileWrite.bad() && fileExists(clientState.logFile)){
                error("ERROR: Client logFile has been opened but was not written to");}
            return;
        }else{
            clientState.logFileWrite << "[" << getTime() << "] RECV: " <<recString <<std::endl;
            if(clientState.logFileWrite.bad() && fileExists(clientState.logFile)){
                error("ERROR: Client logFile has been opened but was not written to");
            return;
            }
        }
    }
}




void client::writeSock(int sockFd, const int* disconnect) {
    ssize_t errNo;
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if(*disconnect != 0)
            return;
        std::cout<<"Please enter the message: ";
        std::string input = "";
        std::getline(std::cin, input);
        input += "\n";
        errNo = write(sockFd, input.c_str(), input.size());
        if (errNo < 0)
            error("ERROR writing to socket");
    }
}








void client::parseArgs(int argc, char *argv[]) {
    //processing arguments with getOpt:

    //Run switch statement below regardless of # of args provided so that configFile variable in struct is populated in either case:
    std::cout<<"\n[Client Variables set by manual args:]"<<std::endl;

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
                clientState.logFile = optarg;
               // std::cout << "Log File has been opened under name: " << optarg << std::endl;
                break;
            }
            default  :
                std::cerr
                        << "Incorrect usage: Incorrect arguments, Client args are in the form: -h hostname -u username -p serverPort -c configFile -t testFile -L logFile";
                exit(0);
        }
    }

}

std::vector<std::string> client::split(std::string const &input){
        std::istringstream buffer(input);
        std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());

        return ret;
    }

bool client::fileExists(std::string &Filename) {
    std::ifstream file(Filename);
    if(!file)
        return false;
    else
        return true;
}

std::string client::getTime() {
    t = time(NULL);
    struct tm* curtime = localtime(&t);
    std::string ctime = asctime(curtime);
    ctime.erase(std::remove(ctime.begin(),ctime.end(),'\n'),ctime.end());

    return ctime;
}


