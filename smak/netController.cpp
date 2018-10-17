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
