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

        /**
         * Sets the away message for the user. If the away message is set, any message sent to the user will
         * automatically reply with the away message.
         * If the user is notified, the away message will not be sent and the notice will be passed on to the user
         * if the away message is set to "here" the user will not be treated as away
         * @param awayMsg "here" if user is present, anything else if they are away
         */
        void setAwayMsg(std::string& awayMsg);

        /**
         * Sets the password of the user.
         * This really doesn't do much other than write it to a file. Because to hell with security
         * @param password The string to use as the user's password.
         */
        void setPassword(std::string& password);

        /**
         * Sets the level of the user
         * either "user" "ops" "sysops" or "admin"
         * @param level "user" "ops" "sysops" "admin"
         */
        void setLevel(std::string& level);

        /**
         * Gives back a shared pointer reference to the underlying tcpUserSocket.
         * This function should really only be used for listening to the port
         * @return A shared pointer to the tcpUserSocket. Intended for listening
         */
        std::shared_ptr<smak::tcpUserSocket> getSession();

        /**
         * Gives back the name of the user
         * There are no nicknames supported
         * @return user's name
         */
        std::string getName() { return username;}

        /**
         * Gives back the away message of the user.
         * If the away message is "here" the user should be treated as present
         * @return "here" if present, anything else if away
         */
        std::string getAwayMsg(){return away;}

        /**
         * Gives back the password in raw for the user.
         * We don't care about security
         * @return raw string for the user's password
         */
        std::string getPassword(){return password;}

        /**
         * Gives back the level of the user as a string
         * "user" "ops" "sysops" "admin"
         * @return Gives back the level of the user as "user" "ops" "sysops" or "admin"
         */
        std::string getLevel(){return level;}

        /**
         * This will disconnect the user when the user's thread is not blocked.
         * Upon the next message received from the user, the message will not be processed, instead the user
         * will be removed from the server and their thread joined.
         */
        void safeDisconnect();

        /**
         * Tells if the user is set to be disconnected at the next not-blocked state
         * True if the user will be disconnected, false if not.
         * @return True if the user will be disconnected when not blocked, false if their session will be continued
         */
        bool disconnect() { return disconnect_flag;}

        /**
         * Tells if the user is banned, ie all priv and channel messages are ignored
         * @return true if banned, false if not
         */
        bool isBanned(){return banned;}

        /**
         * Sets the banned value for the user
         * True if banned false if not
         * @param passed True if banned, false if not
         */
        void setBan(bool passed){banned = passed;};

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

        // This value exists for the kick_user function. If a user is kicked users thread will most likely be
        // caught in the blocking read call. We flip this value, and after the next message received from the client
        // they will be disconnected.
        // disconnect is also used in a more trivial case for QUITE but unlike kick we don't get caught in a blocking
        // call
        bool disconnect_flag;
        bool banned = false;

        std::shared_ptr<smak::tcpUserSocket> session;
    };
}

#endif //SMAK_USER_H
