//
// Created by michael on 10/16/18.
//

#include "netController.h"
#include "../rapidjson/document.h"

#define OP "OP"
#define OP_SETNAME "SETNAME"
#define OP_JOIN "JOIN"
#define OP_MSG "MSG"
#define OP_INITIAL_SETTINGS "SET"
#define OP_PRIVMSG "PRIVMSG"
#define OP_AWAY "AWAY"

#define F_CHANNEL "channel"
#define F_NAME "name"
#define F_MESSAGE "message"
#define F_PASSWORD "password"
#define F_LEVEL "level"


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

        if (op == OP_INITIAL_SETTINGS) {
            //This will be called automatically from client and will set to the name field from .conf file- requirement that each .conf file has a name set
            setInit(jsonDom, fromUser);
        }
        if (op == OP_SETNAME){
            opSetName(jsonDom, fromUser);}
        else if (op == OP_JOIN){
            opJoin(jsonDom, fromUser);}
        else if (op == OP_MSG){
            opMsg(jsonDom, fromUser);
        }
        else if (op==OP_PRIVMSG){
            opPrivMsg(jsonDom, fromUser);
        }
        else if (op==OP_AWAY){
            opAway(jsonDom, fromUser);
        }



    }
    // This is legacy. It just broadcasts messages that are not json ***THIS MAKES THE CLIENT FREAK OUT in an infinite loop
    else {
       // this->broadcastMessage(serverState->getChatLog());
    }
    // TODO: Get rid of this
    serverState->appendToChat(cmd);
}


smak::netController::netController(srvState* state) {
    serverState = state;
}


//TODO: is this method used anymore? - can it be reused for WALLOPS?
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
    assert(jsonDom.HasMember(F_CHANNEL));
    assert(jsonDom[F_CHANNEL].IsString());
    std::string channelName = jsonDom[F_CHANNEL].GetString();

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
    assert(jsonDom.HasMember(F_CHANNEL));
    assert(jsonDom[F_CHANNEL].IsString());
    std::string channelName = jsonDom[F_CHANNEL].GetString();

    assert(jsonDom.HasMember(F_MESSAGE));
    assert(jsonDom[F_MESSAGE].IsString());
    std::string message = fromUser.get()->getName() + ": " + jsonDom[F_MESSAGE].GetString(); //now we know who is saying what in the channel

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
        throw std::string("ERROR: Could not send chat message to specified channel: " + channelName);
    }

}

void smak::netController::opPrivMsg(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    assert(jsonDom.HasMember(F_NAME));
    assert(jsonDom[F_NAME].IsString());
    std::string userName = jsonDom[F_NAME].GetString();

    assert(jsonDom.HasMember(F_MESSAGE));
    assert(jsonDom[F_MESSAGE].IsString());
    std::string message = "[PRIVMSG] from " + fromUser.get()->getName() + ": " + jsonDom[F_MESSAGE].GetString();
    std::string msgWhileAway = "[PRIVMSG] (sent while you were AWAY) from " + fromUser.get()->getName() + ": " + jsonDom[F_MESSAGE].GetString();

    bool sent = false;
    auto allUsers = serverState->getUsers(); //Get all the current users and try to find the one specified in passed userName

    if(userName=="anon"){
        std::string errMsg = "__Cannot send a PRVMSG to anonymous users who have not provided a username or nickname__";
        fromUser.get()->sendString(errMsg);

    }
    else{
    for (auto user : allUsers){
        if(user.get()->getName() == userName){
            if(!user.get()->getAwayMsg().empty()){ //If the desired end user has a non empty AWAY string and has not come back to the keyboard
                std::string away = userName + " Has set their current status to away and the AWAY message is: " + user.get()->getAwayMsg();
                fromUser.get()->sendString(away); //sending user is alerted that their PRVMSG was sent to a client that is away
                user.get()->sendString(msgWhileAway);
            }
            else{           user.get()->sendString(message);} //sending like normal because they are not currently AWAY

            sent = true;
            break;
        }
    }
    if (!sent){

        throw std::string("ERROR: Could not send PRVMSG message to specified user: " + userName + " This user does not exist on the server");

    }
    }

}

void smak::netController::opAway(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    assert(jsonDom.HasMember(F_MESSAGE));
    //assert(jsonDom[F_MESSAGE].IsString()); //THIS COULD BREAK IF AWAY STRING IS EMPTY

    std::string message = jsonDom[F_MESSAGE].GetString();

    if(message == "here"){
        fromUser.get()->setName(""); //clear out the AWAY string to empty if user is no at keyboard again
    }
    else{fromUser.get()->setName(message);}


}

void smak::netController::setInit(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    std::string newName;
    assert(jsonDom.HasMember(F_NAME));
    assert(jsonDom[F_NAME].IsString());
    newName = jsonDom[F_NAME].GetString();
    if(newName.length()==0){
        fromUser.get()->setName("Anon"); //default case for if no username/nickname is specified
    }

    else{fromUser.get()->setName(newName);}


    std::string password;
    assert(jsonDom.HasMember(F_PASSWORD));
    assert(jsonDom[F_PASSWORD].IsString());
    password = jsonDom[F_PASSWORD].GetString();
    fromUser.get()->setPassword(password);

    std::string level;
    assert(jsonDom.HasMember(F_LEVEL));
    assert(jsonDom[F_LEVEL].IsString());
    level = jsonDom[F_LEVEL].GetString();
    fromUser.get()->setLevel(level);
}
