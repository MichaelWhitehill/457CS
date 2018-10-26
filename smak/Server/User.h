//
// Created by michael on 10/25/18.
//

#ifndef SMAK_USER_H
#define SMAK_USER_H


#include <string>
#include "tcpUserSocket.h"

class User {
public:
    User(std::shared_ptr<cs457::tcpUserSocket> session, const std::string& uName = "Anon");
    void closeSession();
    void setName(const std::string& name);
    std::shared_ptr<cs457::tcpUserSocket> getSocket();
    std::string getName() { return username;}

    void sendString(const std::string& toSend);


private:
    std::string username;
    std::shared_ptr<cs457::tcpUserSocket> session;

};


#endif //SMAK_USER_H
