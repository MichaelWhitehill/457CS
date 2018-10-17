//
// Created by michael on 10/16/18.
//

#include <iostream>
#include "srvState.h"

//void srvState::pushBackThread(std::unique_ptr<std::thread> threadPointer) {
//    threadList.push_back(std::move(threadPointer));
//}

srvState::srvState() {
    chatLog = "This is the beaning of the chat log\n";
    sessions = std::vector<std::shared_ptr<cs457::tcpUserSocket>>();
    threadList = std::vector<std::unique_ptr<std::thread>>();
}

void srvState::pushBackSession(std::shared_ptr<cs457::tcpUserSocket> session) {
    sessions.push_back(session);
}

std::vector<std::unique_ptr<std::thread>> srvState::getThreads() {
    return std::vector<std::unique_ptr<std::thread>>();
}

void srvState::appendToChat(const std::string &str) {
    chatLog += str;
}

const std::string& srvState::getChatLog() {
    return chatLog;
}

std::vector<std::shared_ptr<cs457::tcpUserSocket>> srvState::getSessions() {
    return sessions;
}
