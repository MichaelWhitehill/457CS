//
// Created by michael on 10/16/18.
//

#include "netController.h"
#include "../rapidjson/document.h"

#define OP "OP"
#define OP_SETNAME "SETNAME"

void smak::netController::interpret(const std::string &cmd, std::shared_ptr<smak::User> fromUser) {
    // TODO: everything
    // My Dommy's name is jason
    rapidjson::Document jsonDom;
    jsonDom.Parse(cmd.c_str());
    if(jsonDom.IsObject()) {
        // Every message that is a json message should have an op that is a string.
        // Everything else is OP specific
        assert(jsonDom.HasMember(OP));
        assert(jsonDom[OP].IsString());
        std::string op = jsonDom[OP].GetString();

        if (op == OP_SETNAME)
            opSetName(jsonDom, fromUser);

    }
    // This is legacy. It just broadcasts messages that are not json
    else {
        this->broadcastMessage(serverState->getChatLog());
    }
    // TODO: Get rid of this
    serverState->appendToChat(cmd);
}


smak::netController::netController(srvState* state) {
    serverState = state;
}

void smak::netController::broadcastMessage(const std::string &toBroadcast) {
    std::vector<std::shared_ptr<User>> users = serverState->getUsers();
    for (std::shared_ptr<User> user : users){
        user.get()->sendString(toBroadcast);
    }
}

void smak::netController::closeUserConnection(std::shared_ptr<User> userToClose) {
    userToClose.get()->sendString("GOODBYE");
    auto socket = userToClose.get()->getSession();
    socket.get()->closeSocket();
    serverState->removeUser(userToClose);

    unsigned long con_count = serverState->getUsers().size();
    std::string s = "There are " + std::to_string(con_count) + " connections";
    broadcastMessage(s);
}

void smak::netController::opSetName(const rapidjson::Document& jsonDom, std::shared_ptr<User> fromUser) {
    const std::string NAME_FIELD = "name";

    fromUser.get()->sendString("Your name used to be: " + fromUser.get()->getName());

    std::string newName;
    assert(jsonDom.HasMember(NAME_FIELD.c_str()));
    assert(jsonDom[NAME_FIELD.c_str()].IsString());
    newName = jsonDom[NAME_FIELD.c_str()].GetString();

    fromUser.get()->setName(newName);

    fromUser.get()->sendString("Your new name is to be: " + fromUser.get()->getName());


}
