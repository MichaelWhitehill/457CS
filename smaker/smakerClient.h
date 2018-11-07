//
// Created by michael on 11/6/18.
//

#ifndef SMAK_SMAKERCLIENT_H
#define SMAK_SMAKERCLIENT_H


#include <string>
#include <vector>

class smakerClient {
public:
    smakerClient(std::string hostname, int portNo);
    void closeCon();
    std::vector<std::string> sendGetResponces(std::string jsonStr, int responseCount);
    void sendStr(std::string toSend);
    std::string getSrvResponse();
    std::vector<std::string> listenForMessages(uint messageCount);

private:
    int sockfd;

};


#endif //SMAK_SMAKERCLIENT_H
