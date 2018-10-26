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
    std::vector<std::shared_ptr<User>> users = serverState->getUsers();
    for (std::shared_ptr<User> user : users){
        user.get()->sendString(toBroadcast);
    }
}

void netController::closeUserConection(std::shared_ptr<User> userToClose) {
    userToClose.get()->sendString("GOODBYE");
    auto socket = userToClose.get()->getSocket();
    socket.get()->closeSocket();
    serverState->removeUser(userToClose);

    unsigned long con_count = serverState->getUsers().size();
    std::string s = "There are " + std::to_string(con_count) + " connections";
    broadcastMessage(s);
}
