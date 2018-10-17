//
// Created by michael on 10/16/18.
//

#ifndef SMAK_SRVSTATE_H
#define SMAK_SRVSTATE_H


#include <thread>
#include "tcpUserSocket.h"

class srvState {
public:
    srvState(std::vector<std::unique_ptr<std::thread>>* threadList_p);
    /**
     * Adds a session or connection to the list of maintained sessions
     * @param session User socket representing each connected client
     */
    void pushBackSession(std::shared_ptr<cs457::tcpUserSocket> session);
    /**
     * Gives back a list of sessions. We might want to make this const
     * @return List of user sockets, each representing a connected user
     */
    std::vector<std::shared_ptr<cs457::tcpUserSocket>> getSessions();

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
    std::vector<std::shared_ptr<cs457::tcpUserSocket>> sessions;
    // using a pointer for the threadList is UNTESTED
    std::vector<std::unique_ptr<std::thread>>* threadList_p;
    std::string chatLog;


};


#endif //SMAK_SRVSTATE_H
