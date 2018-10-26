//
// Created by michael on 10/16/18.
//

#include "netController.h"
#include "../rapidjson/document.h"

#define OP "OP"
#define OP_SETNAME "SETNAME"

void netController::interpret(const std::string &cmd, std::shared_ptr<User> fromUser) {
    // TODO: everything
    // My Dommy's name is jason
    rapidjson::Document jsonDom;
    jsonDom.Parse(cmd.c_str());
    if(jsonDom.IsObject()){
        assert(jsonDom.HasMember(OP));
        assert(jsonDom[OP].IsString());
        std::string op = jsonDom[OP].GetString();
        if (op == OP_SETNAME)
            opSetName(jsonDom, fromUser);

    } else {
        this->broadcastMessage(serverState->getChatLog());
    }
    serverState->appendToChat(cmd);
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

void netController::opSetName(const rapidjson::Document& jsonDom, std::shared_ptr<User> fromUser) {
    const std::string NAME_FIELD = "Name";

    fromUser.get()->sendString("Your name used to be: " + fromUser.get()->getName());

    std::string newName;
    assert(jsonDom.HasMember(NAME_FIELD.c_str()));
    assert(jsonDom[NAME_FIELD.c_str()].IsString());
    newName = jsonDom[NAME_FIELD.c_str()].GetString();

    fromUser.get()->setName(newName);

    fromUser.get()->sendString("Your new name is to be: " + fromUser.get()->getName());


}
