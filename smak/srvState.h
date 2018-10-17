//
// Created by michael on 10/16/18.
//

#ifndef SMAK_SRVSTATE_H
#define SMAK_SRVSTATE_H


#include <thread>
#include "tcpUserSocket.h"

class srvState {
public:
    srvState();
    // void pushBackThread(std::unique_ptr<std::thread> threadPointer);
    void pushBackSession(std::shared_ptr<cs457::tcpUserSocket> session);
    std::vector<std::unique_ptr<std::thread>> getThreads();
    std::vector<std::shared_ptr<cs457::tcpUserSocket>> getSessions();

    void appendToChat(const std::string& str);
    const std::string& getChatLog();

private:
    //TODO: Add server rules as string (maybe?)

    // maintains list of all connected
    std::vector<std::shared_ptr<cs457::tcpUserSocket>> sessions;
    std::vector<std::unique_ptr<std::thread>> threadList;
    std::string chatLog;


};


#endif //SMAK_SRVSTATE_H
