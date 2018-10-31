//
// Created by michael on 10/30/18.
//

#include <string>
#include "Channel.h"


smak::Channel::Channel(const std::string &channelName) {
    this->channelName = channelName;
}

void smak::Channel::join(std::shared_ptr<smak::User> &user) {
    userList.insert(user);
}

void smak::Channel::leave(std::shared_ptr<const smak::User> &user) {
    userList.erase(userList.begin(), userList.find(user));
}

const std::set<std::shared_ptr<const smak::User>> &smak::Channel::getUsers() {
    return userList;
}

std::string smak::Channel::getName() {
    return channelName;
}

