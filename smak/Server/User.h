//
// Created by michael on 10/25/18.
//

#ifndef SMAK_USER_H
#define SMAK_USER_H


#include <string>
#include "tcpUserSocket.h"

class User {
public:
    /**
     * Constructor for a User instance. This wraps the a tcp user socket
     * @param session a shared pointer to a tcpUser socket
     * @param uName The username for the user, defaulted to anon, probably stays that way
     */
    User(std::shared_ptr<cs457::tcpUserSocket> session, const std::string& uName = "Anon");

    /**
     * Closes the file descriptor of the socket associated with the user
     */
    void closeSession();

    /**
     * Updates the name of the user
     * @param name Name of the user to be updated to
     */
    void setName(const std::string& name);

    std::string getName() { return username;}

    /**
     * Sends  a string to the specified user. It uses multithreading and all that jazz
     * @param toSend const string reference to be sent to the user
     */
    void sendString(const std::string& toSend);


private:
    std::string username;
    std::shared_ptr<cs457::tcpUserSocket> session;
};


#endif //SMAK_USER_H
