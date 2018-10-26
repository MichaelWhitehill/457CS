//
// Created by michael on 10/16/18.
//

#ifndef SMAK_SRVSTATE_H
#define SMAK_SRVSTATE_H


#include <thread>
#include "tcpUserSocket.h"
#include "User.h"

class srvState {
public:
    srvState(std::vector<std::unique_ptr<std::thread>>* threadList_p);
    /**
     * Adds a session or connection to the list of maintained sessions
     * @param session User socket representing each connected client
     */
    void pushBackSession(std::shared_ptr<User> user);

    std::vector<std::shared_ptr<User>> getUsers();

    void removeUser(std::shared_ptr<User>);

    /**
     * appends a const string ref to the current chat log
     * @param str data to be appended to the chat log
     */
    void appendToChat(const std::string& str);
    /**
     * Gives back a reference to the chat log. It's const
     * @return Const reference to the current chat log
     */
    const std::string& getChatLog();


private:
    //TODO: Add server rules as string (maybe?)

    // maintains list of all connected
    // std::vector<std::shared_ptr<cs457::tcpUserSocket>> sessions;
    std::vector<std::shared_ptr<User>> users;
    // using a pointer for the threadList is UNTESTED
    std::vector<std::unique_ptr<std::thread>>* threadList_p;
    std::string chatLog;


};


#endif //SMAK_SRVSTATE_H
