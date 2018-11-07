//
// Created by michael on 11/6/18.
//

#include <fstream>
#include <iostream>
#include "smakerMain.h"
#include "smakerClient.h"
#include "../smak/main.h"

int smakerMain::main(int argc, char ** argv) {
    std::ifstream testFile(argv[1]);
    std::string portLine;
    std::string serverLine;

    getline(testFile, portLine);
    int port;
    port = std::stoi(portLine);
    getline(testFile, serverLine);

    std::cout<<"Port: " << port <<"\n";
    std::cout<<"Server: " << serverLine << "\n";

    smakerClient smaker(serverLine, port);
    return runSmakerFile(testFile, smaker);
}

int smakerMain::runSmakerFile(std::ifstream& smakerFile, smakerClient smaker) {
    std::string currentLine;
    std::string currentSendString;
    int allGood = 0;

    std::getline(smakerFile, currentSendString);
    currentSendString = currentSendString.substr(6, currentSendString.length());
    std::vector<std::string> recStrings;
    while(std::getline(smakerFile, currentLine)){
        if (currentLine.substr(0, 4) == "SEND"){
            int retval = runSmakerTest(currentSendString, recStrings, smaker);
            if (retval != 0)
                allGood = 1;
            currentSendString = currentLine.substr(6, currentLine.length());
            recStrings.clear();
        } else if (currentLine.substr(0, 3) == "REC"){
            recStrings.push_back(currentLine.substr(5, currentLine.length()));
        }
    }
    int retval = runSmakerTest(currentSendString, recStrings, smaker);
    if (retval != 0)
        return retval;
    return allGood;
}

int smakerMain::runSmakerTest(std::string sendString, std::vector<std::string> responseStrings, smakerClient smaker) {
    std::cout<<"Testing string: " << sendString << std::endl;
    std::vector<std::string> realStrings = smaker.sendGetResponces(sendString, responseStrings.size());
    for (int i = 0; i < realStrings.size(); ++i){
        if (realStrings[i] != responseStrings[i]){
            std::cout<<"\tFAILED"<<std::endl;
            std::cout<<"\t\tExpected response: " << responseStrings[i] <<std::endl;
            std::cout<<"\t\tActual response:   " << realStrings[i] <<std::endl;
            return 1;
        }
    }
    std::cout<<"\tPASSED"<<std::endl;
    return 0;
}
