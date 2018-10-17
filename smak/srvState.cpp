//
// Created by michael on 10/16/18.
//

#include <iostream>
#include "srvState.h"

static std::vector<std::unique_ptr<std::thread>> threadList;

void srvState::pushBackSession(std::shared_ptr<cs457::tcpUserSocket> session) {
    sessions.push_back(session);
}


void srvState::appendToChat(const std::string &str) {
    chatLog += str;
}

const std::string& srvState::getChatLog() {
    return chatLog;
}

srvState::srvState(std::vector<std::unique_ptr<std::thread>> *threadList_p) {
    this->threadList_p = threadList_p;
}

std::vector<std::shared_ptr<cs457::tcpUserSocket>> srvState::getSessions() {
    return sessions;
}

