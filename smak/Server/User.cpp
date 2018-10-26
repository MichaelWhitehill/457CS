//
// Created by michael on 10/25/18.
//

#include <thread>
#include "User.h"

User::User(std::shared_ptr<cs457::tcpUserSocket> session, const std::string &uName) {
    this->session = session;
    username = uName;
}

void User::closeSession() {
    session.get()->closeSocket();
}

std::shared_ptr<cs457::tcpUserSocket> User::getSocket() {
    return session;
}

void User::setName(const std::string& name) {
    username = name;
}

void User::sendString(const std::string &toSend) {
    std::thread senderThread = std::thread(&cs457::tcpUserSocket::sendString, session, toSend, true);
    // Every thread must be joined/closed before continuing to the next
    senderThread.join();
}
