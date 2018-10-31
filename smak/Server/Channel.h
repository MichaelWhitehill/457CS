//
// Created by michael on 10/30/18.
//

#ifndef SMAK_CHANNEL_H
#define SMAK_CHANNEL_H

#include <string>
#include <set>
#include "User.h"


namespace smak{
    class Channel {
    public:
        Channel(const std::string& channelName);
        void join(std::shared_ptr<smak::User>& user);
        void leave(std::shared_ptr<smak::User>& user);
        const std::set<std::shared_ptr<smak::User>>& getUsers();
        void const sendMessage(const std::string& message);
        std::string getName();

    private:
        std::set<std::shared_ptr<smak::User>> userList;
        std::string channelName;
    };
}



#endif //SMAK_CHANNEL_H
