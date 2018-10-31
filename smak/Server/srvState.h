//
// Created by michael on 10/16/18.
//

#ifndef SMAK_SRVSTATE_H
#define SMAK_SRVSTATE_H


#include <thread>
#include "tcpUserSocket.h"
#include "User.h"
#include "Channel.h"

namespace smak{
    class srvState {
    public:
        srvState(std::vector<std::unique_ptr<std::thread>>* threadList_p);
        /**
         * Adds a session or connection to the list of maintained sessions
         * @param session User representing each connected client
         */
        void pushBackSession(std::shared_ptr<User> user);

        /**
         * Gives back a vector of connected users
         * @return vector of shared pointers to connected users
         */
        std::vector<std::shared_ptr<User>> getUsers();

        /**
         * Removes the reference to the user given
         * @param user The user to be forgotten
         */
        void removeUser(std::shared_ptr<User> user);

        /**
         * appends a const string ref to the current chat log
         * @param str data to be appended to the chat log
         */
        void appendToChat(const std::string& str);

        void addChannel(smak::Channel channel);

        std::vector<smak::Channel>& getChannels();

        /**
         * Gives back a reference to the chat log. It's const
         * @return Const reference to the current chat log
         */
        const std::string& getChatLog();


    private:
        //TODO: Add server rules as string (maybe?)

        // maintains list of all connected users
        std::vector<std::shared_ptr<User>> users;
        // using a pointer for the threadList is UNTESTED
        std::vector<std::unique_ptr<std::thread>>* threadList_p;
        // A list of channels on the server
        std::vector<smak::Channel> channelList;
        std::string chatLog;
    };
}

#endif //SMAK_SRVSTATE_H
