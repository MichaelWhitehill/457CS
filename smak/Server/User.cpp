//
// Created by michael on 10/25/18.
//

#include <thread>
#include "User.h"

smak::User::User(std::shared_ptr<smak::tcpUserSocket> session, const std::string &uname) {
    this->session = session;
    username = uname;
    disconnect_flag = false;
}

void smak::User::closeSession() {
    session.get()->closeSocket();
}

void smak::User::setName(const std::string& name) {
    this->username = name;
}

const void smak::User::sendString(const std::string &toSend) {
    std::thread senderThread = std::thread(&smak::tcpUserSocket::sendString, session, toSend, true);
    // Every thread must be joined/closed before continuing to the next
    senderThread.join();
}

//Session is an instance of TCPUser Socket for this client
std::shared_ptr<smak::tcpUserSocket> smak::User::getSession() {
    return session;
}

void smak::User::setAwayMsg(std::string& awayMsg) {
    this->away = awayMsg;
}

void smak::User::setPassword(std::string& pass) {
    this->password = pass;
}

void smak::User::setLevel(std::string& lev) {
    this->level = lev;
}

void smak::User::safeDisconnect() {
    disconnect_flag = true;
}

