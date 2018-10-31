//
// Created by michael on 10/30/18.
//

#include <string>
#include "Channel.h"


smak::Channel::Channel(const std::string &channelName) {
    this->channelName = channelName;
}

void smak::Channel::join(std::shared_ptr<const smak::User> &user) {
    userList.push_back(user);
}

void smak::Channel::leave(std::shared_ptr<const smak::User> &user) {
    for (size_t i = 0; i < userList.size(); ++i){
        if(user == userList[i]){
            userList.erase(userList.begin() + i);
        }
    }
}

const std::vector<std::shared_ptr<const smak::User>> &smak::Channel::getUsers() {
    return userList;
}

