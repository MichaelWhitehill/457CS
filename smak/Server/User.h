//
// Created by michael on 10/25/18.
//

#ifndef SMAK_USER_H
#define SMAK_USER_H


#include <string>
#include "tcpUserSocket.h"
namespace smak{
    class User {
    public:
        /**
         * Constructor for a User instance. This wraps the a tcp user socket
         * @param session a shared pointer to a tcpUser socket
         * @param uName The username for the user, defaulted to anon, probably stays that way
         */
        User(std::shared_ptr<smak::tcpUserSocket> session, const std::string &uname = "Anon");

        /**
         * Closes the file descriptor of the socket associated with the user
         */
        void closeSession();

        /**
         * Updates the name of the user
         * @param name Name of the user to be updated to
         */
        void setName(const std::string& name);

        void setAwayMsg(std::string& awayMsg);

        void setPassword(std::string& password);

        void setLevel(std::string& level);

        /**
         * Gives back a shared pointer reference to the underlying tcpUserSocket.
         * This function should really only be used for listening to the port
         * @return A shared pointer to the tcpUserSocket. Intended for listening
         */
        std::shared_ptr<smak::tcpUserSocket> getSession();

        std::string getName() { return username;}

        std::string getAwayMsg(){return away;}

        std::string getPassword(){return password;}

        std::string getLevel(){return level;}



        /**
         * Sends  a string to the specified user. It uses multithreading and all that jazz
         * @param toSend const string reference to be sent to the user
         */
        const void sendString(const std::string& toSend);



    private:
        std::string username;
        std::string away;
        std::string password;
        std::string level;
        //bool banned; ??

        std::shared_ptr<smak::tcpUserSocket> session;
    };
}

#endif //SMAK_USER_H
