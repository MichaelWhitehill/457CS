//
// Created by michael on 10/30/18.
//

#ifndef SMAK_CHANNEL_H
#define SMAK_CHANNEL_H

#include <string>
#include "User.h"


namespace smak{
    class Channel {
    public:
        Channel(const std::string& channelName);
        void join(std::shared_ptr<const smak::User>& user);
        void leave(std::shared_ptr<const smak::User>& user);
        const std::vector<std::shared_ptr<const smak::User>>& getUsers();

    private:
        std::vector<std::shared_ptr<const smak::User>> userList;
        std::string channelName;
    };
}



#endif //SMAK_CHANNEL_H
