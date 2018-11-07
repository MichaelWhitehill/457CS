//
// Created by michael on 11/6/18.
//

#ifndef SMAK_SMAKERMAIN_H
#define SMAK_SMAKERMAIN_H


#include "smakerClient.h"

class smakerMain {
public:
    static int main(int argc, char** argv);

private:
    static int runSmakerFile(std::ifstream& smakerFile, smakerClient smaker);
    static int runSmakerTest(std::string sendString, std::vector<std::string> responseStrings, smakerClient smaker);

};


#endif //SMAK_SMAKERMAIN_H
