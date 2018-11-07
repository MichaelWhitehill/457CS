//
// Created by michael on 10/16/18.
//

#ifndef SMAK_NETCONTROLLER_H
#define SMAK_NETCONTROLLER_H

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
#define OP_LOCK_CHANNEL "LOCK"
#define OP_UNLOCK_CHANNEL "UNLOCK"
#define OP_KNOCK "KNOCK"
#define OP_INVITE "INVITE"
#define OP_RULES "RULES"
#define OP_VERSION "VERSION"
#define OP_TOPIC "TOPIC"
#define OP_LIST "LIST"
#define OP_PART "PART"
#define OP_ISON "ISON"
#define OP_SILENCE "SILENCE"
#define OP_PASS "PASS"
#define OP_NOTICE "NOTICE"

#define F_CHANNEL "channel"
#define F_NAME "name"
#define F_MESSAGE "message"
#define F_PASSWORD "password"
#define F_LEVEL "level"
#define F_TOPIC "topic"


#include <string>
#include "srvState.h"
#include "../rapidjson/document.h"
namespace smak{
    class netController {
    public:
        /**
         * Basic constructor, could use some revamping
         * @param state Pointer to an srvState instance.
         */
        netController(smak::srvState* state);

        /**
         * Takes in a message as a JSON Dom and performs the operation(s) specified
         * @param cmd a string representation of a JSON Dom from the client
         * @param fromSession A reference to the user who sent the request
         */
        void interpret(const std::string& cmd, std::shared_ptr<smak::User> fromSession);

        /**
         * Closes the connection to a tcpUser socket by closing the FD, and removing it from the srvState list of clients
         * @param closedClient
         */
        void closeUserConnection(std::shared_ptr<smak::User> userToClose);

    private:
        // Should contain all generic info and list of connections
        smak::srvState* serverState;

        /**
         * broadcasts a message to all connected sessions
         * @param toBroadcast the string to be sent
         */
        void broadcastMessage(const std::string& toBroadcast);

        /**
         * Implementation of the SETNAME OP. It sets the name of the from user to the one specified in the json
         * @param jsonDom {"OP":"SETNAME","Name":"USERNAME"}
         * @param fromUser
         */
        void opSetName(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opJoin(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opMsg(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opPrivMsg(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opAway(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void setInit(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opInfo(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opPing(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opOper(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opUsers(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void opTime(const rapidjson::Document& jsonDom, std::shared_ptr<smak::User> fromUser);

        void  opQuit(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opKick(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opKill(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opLockChannel(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {setChannelLock(jsonDom, fromUser, true);}

        void opUnlockChannel(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser) {setChannelLock(jsonDom, fromUser, false);}

        void opKnock(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opInvite(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void setChannelLock(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser, bool lockState);

        void opRules(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opVersion(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opTopic(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opList(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opPart(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opIson(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opSilence(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opPass(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

        void opNotice(const rapidjson::Document &jsonDom, std::shared_ptr<smak::User> fromUser);

    };
}

#endif //SMAK_NETCONTROLLER_H
