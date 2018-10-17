//
// Created by michael on 10/16/18.
//

#ifndef SMAK_NETCONTROLLER_H
#define SMAK_NETCONTROLLER_H

#include <string>
#include "srvState.h"

class netController {
public:
    netController(srvState* state);

    /**
     * Takes incoming network traffic and determines the function to call
     * @param cmd incoming command
     */
    void interpret(const std::string& cmd);

private:
    srvState* serverState;

    /**
     * broadcasts a message to all connected sessions
     * @param toBroadcast the string to be sent
     */
    void broadcastMessage(const std::string& toBroadcast);

};


#endif //SMAK_NETCONTROLLER_H
