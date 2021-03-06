//
// Created by michael on 10/30/18.
//

#include <string>
#include "Channel.h"


smak::Channel::Channel(const std::string &channelName) {
    this->channelName = channelName;
    this->topic = "Call set topic to set a channel topic.";
    inviteOnly = false;
}

void smak::Channel::join(std::shared_ptr<smak::User> &user) {
    // remove the user from the invited users set, if they existed in it
    invitedUsers.erase(user);
    userList.insert(user);
}

void smak::Channel::leave(std::shared_ptr<smak::User> &user) {
    // this is safe if the user is not in the set
    userList.erase(user);
}

const std::set<std::shared_ptr<smak::User>> &smak::Channel::getUsers() {
    return userList;
}

std::string smak::Channel::getName() {
    return channelName;
}

void const smak::Channel::sendMessage(const std::string &message) {
    for (std::shared_ptr<smak::User> user : userList){
        user.get()->sendString(message);
    }
}

void smak::Channel::setInviteOnly(bool inviteOnly) {
    this->inviteOnly = inviteOnly;
}

void smak::Channel::inviteUser(std::shared_ptr<smak::User> user) {
    invitedUsers.insert(user);
}

std::set<std::shared_ptr<smak::User>> smak::Channel::getInvitedUsers() {
    return invitedUsers;
}

bool smak::Channel::isInviteOnly() {
    return inviteOnly;
}

std::string smak::Channel::getTopic() {
    return topic;
}

void smak::Channel::setTopic(const std::string& passed) {
    topic = passed;
}

