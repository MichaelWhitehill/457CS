//
// Created by michael on 10/25/18.
//

#include <thread>
#include "User.h"

smak::User::User(std::shared_ptr<smak::tcpUserSocket> session, const std::string &uName) {
    this->session = session;
    username = uName;
}

void smak::User::closeSession() {
    session.get()->closeSocket();
}

void smak::User::setName(const std::string& name) {
    username = name;
}

void smak::User::sendString(const std::string &toSend) {
    std::thread senderThread = std::thread(&smak::tcpUserSocket::sendString, session, toSend, true);
    // Every thread must be joined/closed before continuing to the next
    senderThread.join();
}

std::shared_ptr<smak::tcpUserSocket> smak::User::getSession() {
    return session;
}
