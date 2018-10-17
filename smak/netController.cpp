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
    auto sessions = serverState->getSessions();
    for (std::shared_ptr<cs457::tcpUserSocket> session : sessions){
        // thread childT1(&cs457::tcpUserSocket::sendString,clientSocket.get(),msg,true);
        std::thread senderThread = std::thread(&cs457::tcpUserSocket::sendString, session, toBroadcast, true);
        senderThread.join();
    }
}
