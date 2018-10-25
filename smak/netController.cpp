//
// Created by michael on 10/16/18.
//

#include "netController.h"

void netController::interpret(const std::string &cmd) {
    // TODO: everything
    serverState->appendToChat(cmd);
    this->broadcastMessage(serverState->getChatLog());
}


netController::netController(srvState* state) {
    serverState = state;
}

void netController::broadcastMessage(const std::string &toBroadcast) {
    // Get the list of sessions or connected users
    auto sessions = serverState->getSessions();
    for (std::shared_ptr<cs457::tcpUserSocket> session : sessions){
        // for each user we need to spin up a thread to send them the message
        std::thread senderThread = std::thread(&cs457::tcpUserSocket::sendString, session, toBroadcast, true);
        // Every thread must be joined/closed before continuing to the next
        senderThread.join();
    }
}

void netController::closeConnection(std::shared_ptr<cs457::tcpUserSocket> closedClient) {
    std::thread childT1(&cs457::tcpUserSocket::sendString,closedClient.get(),"GOODBYE",true);
    childT1.join();

    closedClient->closeSocket();
    serverState->removeSession(closedClient);
    unsigned long con_count = serverState->getSessions().size();
    std::string s = "There are " + std::to_string(con_count) + " connections";
    broadcastMessage(s);
}
