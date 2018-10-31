//
// Created by michael on 10/16/18.
//

#include <iostream>
#include "srvState.h"

static std::vector<std::unique_ptr<std::thread>> threadList;

void smak::srvState::pushBackSession(std::shared_ptr<User> session) {
    users.push_back(session);
}


void smak::srvState::appendToChat(const std::string &str) {
    chatLog += str;
}

const std::string& smak::srvState::getChatLog() {
    return chatLog;
}

smak::srvState::srvState(std::vector<std::unique_ptr<std::thread>> *threadList_p) {
    this->threadList_p = threadList_p;
}

std::vector<std::shared_ptr<smak::User>> smak::srvState::getUsers() {
    return users;
}

void smak::srvState::removeUser(std::shared_ptr<User> user) {
    for (size_t i = 0; i < users.size(); ++i){
        if(user == users[i]){
            users.erase(users.begin() + i);
        }
    }
}

void smak::srvState::addChannel(smak::Channel channel) {
    channelList.push_back(channel);
}

std::vector<smak::Channel> &smak::srvState::getChannels() {
    return channelList;
}



