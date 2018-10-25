//
// Created by michael on 10/16/18.
//

#ifndef SMAK_NETCONTROLLER_H
#define SMAK_NETCONTROLLER_H

#include <string>
#include "srvState.h"

class netController {
public:
    /**
     * Basic constructor, could use some revamping
     * @param state Pointer to an srvState instance.
     */
    netController(srvState* state);

    /**
     * Takes incoming network traffic and determines the function to call
     * @param cmd incoming command
     */
    void interpret(const std::string& cmd);

    /**
     * Closes the connection to a tcpUser socket by closing the FD, and removing it from the srvState list of clients
     * @param closedClient
     */
    void closeConnection(std::shared_ptr<cs457::tcpUserSocket> closedClient);

private:
    // Should contain all generic info and list of connections
    srvState* serverState;

    /**
     * broadcasts a message to all connected sessions
     * @param toBroadcast the string to be sent
     */
    void broadcastMessage(const std::string& toBroadcast);

};


#endif //SMAK_NETCONTROLLER_H