//
// Created by michael on 10/4/18.
//

#include <iostream>
#include "Server.h"
#include "client.h"

int main (int argc, char* argv[]){
    if (argc < 2){
        std::cerr<<"Not enough arguments given. Expected [srv, clt], [srv args, client args]"<<std::endl;
        return 1;
    }

    int newArgc = argc -1;
    char* newArgv[newArgc];
    for (int i = 0; i < argc-1; ++i){
        newArgv[i] = argv[i];
    }
    std::string toStart = argv[argc-1];
    if(toStart == "srv"){
        return Server::serverMain(newArgc, newArgv);
    }
    else if(toStart == "clt"){
        return client::clientMain(newArgc, newArgv);
    }

}