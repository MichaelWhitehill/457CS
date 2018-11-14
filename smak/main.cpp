//
// Created by michael on 10/4/18.
//

#include <iostream>
#include "main.h"
#include "Client/client.h"
#include "Server/driver.h"
#include "../smaker/smakerMain.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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
        return smak::driver::driverMain(newArgc, newArgv);
    }
    else if(toStart == "clt"){
        return client::clientMain(newArgc, newArgv);
        //NEED TO PASS PORT # and config file path
    }
    else if(toStart == "smaker"){
        return smakerMain::main(newArgc, newArgv);
    }
}
void error(const char *msg) {
    perror(msg);
    exit(1);
}

std::vector<std::string> split(std::string const &input) {
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());

    return ret;
}
