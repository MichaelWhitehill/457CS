//
// Created by michael on 10/16/18.
//

#include "netController.h"
#include "../rapidjson/document.h"

#define OP "OP"
#define OP_SETNAME "SETNAME"
#define OP_JOIN "JOIN"
#define OP_MSG "MSG"
#define OP_INITIAL_SETTINGS "USER"
#define OP_PRIVMSG "PRIVMSG"
#define OP_AWAY "AWAY"
#define OP_INFO "INFO"
#define OP_PING "PING"
#define OP_OPER "OPER"
#define OP_USERS "USERS"
#define OP_TIME "TIME"
#define OP_QUIT "QUIT"
#define OP_KICK "KICK"
#define OP_KILL "KILL"
#define OP_LOCK_CHANNEL "LOCKCHANNEL"
#define OP_UNLOCK_CHANNEL "UNLOCKCHANNEL"
#define OP_KNOCK "KNOCK"
#define OP_INVITE "INVITE"

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
            //THIS USES THE 'USER' OP CODE
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
        else if (op==OP_INFO){
            opInfo(jsonDom, fromUser);
        }
        else if (op==OP_PING){
            opPing(jsonDom, fromUser);
        }
        else if (op==OP_OPER){
            opOper(jsonDom, fromUser);
        }
        else if (op==OP_USERS){
            opUsers(jsonDom, fromUser);
        }
        else if (op==OP_TIME){
            opTime(jsonDom, fromUser);
        }
        else if (op == OP_QUIT){
            opQuit(jsonDom, fromUser);
        }
        else if (op == OP_KICK){
            opKick(jsonDom, fromUser);
        }
        else if (op == OP_KILL){
            opKill(jsonDom, fromUser);
        }
        else if (op == OP_LOCK_CHANNEL){
            opLockChannel(jsonDom, fromUser);
        }
        else if (op == OP_UNLOCK_CHANNEL) {
            opUnlockChannel(jsonDom, fromUser);
        }
        else if (op == OP_KNOCK) {
            opKnock(jsonDom, fromUser);
        }
        else if (op == OP_INVITE){
            opInvite(jsonDom, fromUser);
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
    for(const std::shared_ptr<smak::Channel> &channel : serverState->getChannels()){
        channel.get()->leave(userToClose);
    }
    userToClose.get()->sendString("GOODBYE");
    auto socket = userToClose.get()->getSession();
    socket.get()->closeSocket();
    serverState->removeUser(userToClose);

    unsigned long con_count = serverState->getUsers().size();
    std::string s = "There are " + std::to_string(con_count) + " connections";
    broadcastMessage(s);
}

void smak::netController::opSetName(const rapidjson::Document& jsonDom, std::shared_ptr<User> fromUser) {
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
    for (auto channel : existingChannels){
        if(channel.get()->getName() == channelName){
            if (channel.get()->isInviteOnly()){
                for (std::shared_ptr<smak::User> invitedUser : channel.get()->getInvitedUsers()){
                    if (invitedUser.get()->getName() == fromUser.get()->getName()){
                        channel.get()->join(fromUser);
                        fromUser.get()->sendString("You joined the invite only channel: " + channelName);
                        return;
                    }
                }
                fromUser.get()->sendString(channelName + "is invite only. You are not invited.");
                return;
            }
            channel.get()->join(fromUser);
            fromUser.get()->sendString("You joined channel: " + channelName);
            return;
        }
    }
    smak::Channel channel(channelName);
    channel.join(fromUser);
    fromUser.get()->sendString("You created channel: " + channelName);
    serverState->addChannel(make_shared<smak::Channel>(channel));

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

    if(userName=="Anon"){
        std::string errMsg = "__Cannot send a PRVMSG to anonymous users who have not provided a username or nickname__";
        fromUser.get()->sendString(errMsg);
    }
    else{
    for (auto user : allUsers){
        if(user.get()->getName() == userName){
            if(user.get()->getAwayMsg()!="here"){ //If the desired end user has a non empty AWAY string and has not come back to the keyboard
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
    assert(jsonDom[F_MESSAGE].IsString());

    std::string message = jsonDom[F_MESSAGE].GetString();

    fromUser.get()->setAwayMsg(message);


}

void smak::netController::setInit(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    std::string newName;
    assert(jsonDom.HasMember(F_NAME));
    assert(jsonDom[F_NAME].IsString());
    newName = jsonDom[F_NAME].GetString();
    if(newName.empty()){
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

    std::string default_val = "here";
    fromUser.get()->setAwayMsg(default_val);
}

void smak::netController::opInfo(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    std::string Name = serverState->getName();
    std::string beginTime = serverState->getStartTime();
    std::string curTime = serverState->getTime();

    fromUser.get()->sendString("The server you are on is: " + Name + "\nThis server has been running since: "+beginTime+"\nThe current time is: "+curTime+"\nFor more info use OP code LIST");
}

void smak::netController::opPing(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    std::string Name = serverState->getName();
    std::string beginTime = serverState->getStartTime();

    fromUser.get()->sendString("[PONG] Your connection is good, server is: "+Name+"\nThis server has been running since: "+beginTime);
}

void smak::netController::opOper(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {

    std::string level = fromUser.get()->getLevel();
    std::string name = fromUser.get()->getName();
    std::string channels;

    if(level=="admin"||level=="sysop"){
        fromUser.get()->sendString("You are: "+name+" your level is: "+level+"\nFor info about other users use OP USERHOST for specific user or USERS fo all");
    }
    else if(level == "channelop"){ //Find all the servers you are on
        auto existingChannels = serverState->getChannels();
        for (auto channel: existingChannels){
            for (auto user : channel->getUsers()){
                if (user.get()->getName() == fromUser.get()->getName()) {
                    channels.append(channel->getName()+", ");
                }
            }
        }
        fromUser.get()->sendString("You are: "+name+" your level is: "+level+" and you have admin channel control over channel(s): "+channels+"\nFor info about other users use OP USERHOST for specific user or USERS fo all");
    }else{fromUser.get()->sendString("You are: "+name+" your level is: "+level+" you do not have any admin or operator privileges"+"\nFor info about other users use OP USERHOST for specific user or USERS fo all");}




}

void smak::netController::opUsers(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
        std::string userList = "All current users on server: \n\n";

        for(auto specUser: serverState->getUsers()){
            std::string user = "Name: "+specUser->getName()+" ,Level: "+specUser->getLevel();
            if(specUser->getAwayMsg()!="here"){user.append(" -Currently AWAY");}
            userList.append(user+"\n");
        }
        fromUser.get()->sendString(userList+"\nFor info on users on a specific channel use OP WHO");
}

void smak::netController::opTime(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    std::string Time = serverState->getTime();
    fromUser.get()->sendString("Current Time on Server is: "+Time+"\nFor more server Info use OP INFO");
}

void smak::netController::opQuit(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser){
    fromUser.get()->safeDisconnect();
}

void smak::netController::opKick(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    std::string kickUserName;
    assert(jsonDom.HasMember(F_NAME));
    assert(jsonDom[F_NAME].IsString());
    kickUserName = jsonDom[F_NAME].GetString();

    assert(jsonDom.HasMember(F_CHANNEL));
    assert(jsonDom[F_CHANNEL].IsString());
    std::string channelName = jsonDom[F_CHANNEL].GetString();

    for (std::shared_ptr<smak::Channel> channel : serverState->getChannels()){
        if (channel.get()->getName() == channelName){
            for (std::shared_ptr<smak::User> user : channel.get()->getUsers()) {
                if (user.get()->getName() == kickUserName){
                    channel.get()->leave(user);
                    fromUser.get()->sendString("User was kicked from channel");
                    return;
                }
            }
        }
    }
    fromUser.get()->sendString("Could not find any user named: " + kickUserName + " in channel: " + channelName);
}

void smak::netController::opKill(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    std::string killUserName;
    assert(jsonDom.HasMember(F_NAME));
    assert(jsonDom[F_NAME].IsString());
    killUserName = jsonDom[F_NAME].GetString();

    for(std::shared_ptr<smak::User> user : serverState->getUsers()){
        if (user.get()->getName() == killUserName){
            user.get()->sendString("You have been removed from the server.");
            user.get()->safeDisconnect();
            return;
        }
    }
    fromUser.get()->sendString("Could not find user: " + killUserName);
}

void smak::netController::setChannelLock(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser, bool lockState) {
    assert(jsonDom.HasMember(F_CHANNEL));
    assert(jsonDom[F_CHANNEL].IsString());
    std::string channelName = jsonDom[F_CHANNEL].GetString();

    for(auto channel : serverState->getChannels()){
        if (channel.get()->getName() == channelName){
            channel.get()->setInviteOnly(lockState);
            fromUser.get()->sendString("Channel lock set for: " + channelName);
            return;
        }
    }
    fromUser.get()->sendString("Could not set channel lock for: " + channelName);
}

void smak::netController::opKnock(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    assert(jsonDom.HasMember(F_CHANNEL));
    assert(jsonDom[F_CHANNEL].IsString());
    std::string channelName = jsonDom[F_CHANNEL].GetString();
    for(auto channel : serverState->getChannels()){
        if (channel.get()->getName() == channelName){
            for (std::shared_ptr<smak::User> user  : channel.get()->getUsers()){
                user.get()->sendString(fromUser.get()->getName() + " would like to join: " + channelName);
            }
            fromUser.get()->sendString("Knocks sent to users of channel:  " + channelName);
            return;
        }
    }
    fromUser.get()->sendString("Error knocking on channel");

}

void smak::netController::opInvite(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {
    // get the user to invite
    assert(jsonDom.HasMember(F_NAME));
    assert(jsonDom[F_NAME].IsString());
    std::string inviteUserName = jsonDom[F_NAME].GetString();
    // get the channel to invite them to
    assert(jsonDom.HasMember(F_CHANNEL));
    assert(jsonDom[F_CHANNEL].IsString());
    std::string channelName = jsonDom[F_CHANNEL].GetString();

    std::shared_ptr<smak::User> inviteUser = make_shared<smak::User>(nullptr);
    for(auto user : serverState->getUsers()){
        if (user.get()->getName() == inviteUserName){
            inviteUser = make_shared<smak::User>(*user.get());
            break;
        }
    }
    // TODO: Make this not die with bad usernames
    if (inviteUser.get() == nullptr){
        fromUser.get()->sendString("Could not find user: " + inviteUserName);
        return;
    }

    for (auto channel : serverState->getChannels()){
        if(channel.get()->getName() == channelName){
            channel.get()->inviteUser(inviteUser);
            inviteUser.get()->sendString("You've been invited to channel: " + channelName);
            fromUser.get()->sendString("Invited user: " + inviteUserName + " to channel: " + channelName);
            return;
        }
    }
    fromUser.get()->sendString("Could not find channel: " + channelName);
}



