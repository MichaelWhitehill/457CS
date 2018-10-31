//
// Created by michael on 10/16/18.
//

#include "netController.h"
#include "../rapidjson/document.h"

#define OP "OP"
#define OP_SETNAME "SETNAME"
#define OP_JOIN "JOIN"
#define OP_MSG "MSG"


#define F_CANNEL "channel"
#define F_NAME "name"
#define F_MESSAGE "message"

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
        else if (op == OP_JOIN)
            opJoin(jsonDom, fromUser);
        else if (op == OP_MSG){
            opMsg(jsonDom, fromUser);
        }


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
    assert(jsonDom.HasMember(F_NAME));
    assert(jsonDom[F_NAME].IsString());
    newName = jsonDom[F_NAME].GetString();

    fromUser.get()->setName(newName);

    fromUser.get()->sendString("Your new name is to be: " + fromUser.get()->getName());


}

void smak::netController::opJoin(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser) {
    assert(jsonDom.HasMember(F_CANNEL));
    assert(jsonDom[F_CANNEL].IsString());
    std::string channelName = jsonDom[F_CANNEL].GetString();

    auto existingChannels = serverState->getChannels();
    bool joined = false;
    for (auto channel : existingChannels){
        if(channel.get()->getName() == channelName){
            channel.get()->join(fromUser);
            joined = true;
            fromUser.get()->sendString("You joined channel: " + channelName);
            break;
        }
    }
    if (!joined){
        smak::Channel channel(channelName);
        channel.join(fromUser);
        fromUser.get()->sendString("You created channel: " + channelName);
        serverState->addChannel(make_shared<smak::Channel>(channel));
    }
}

void smak::netController::opMsg(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser){
    assert(jsonDom.HasMember(F_CANNEL));
    assert(jsonDom[F_CANNEL].IsString());
    std::string channelName = jsonDom[F_CANNEL].GetString();

    assert(jsonDom.HasMember(F_MESSAGE));
    assert(jsonDom[F_MESSAGE].IsString());
    std::string message = jsonDom[F_MESSAGE].GetString();

    auto existingChannels = serverState->getChannels();

    bool sent = false;
    for (auto channel : existingChannels){
        if (channel.get()->getName() == channelName){
            channel.get()->sendMessage(message);
            sent = true;
            break;
        }
    }
    if (!sent){
        throw std::string("Could not send message to chat: " + channelName);
    }

}
