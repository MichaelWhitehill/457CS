//
// Created by michael on 10/16/18.
//

#ifndef SMAK_NETCONTROLLER_H
#define SMAK_NETCONTROLLER_H

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
    };
}

#endif //SMAK_NETCONTROLLER_H
