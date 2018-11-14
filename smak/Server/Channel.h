//
// Created by michael on 10/30/18.
//

#ifndef SMAK_CHANNEL_H
#define SMAK_CHANNEL_H

#include <string>
#include <set>
#include "User.h"


namespace smak{
    class Channel {
    public:
        /**
         * Standard constructor, expects the channel name as an argument
         * @param channelName Takes the channelName argument and stores it as the channel name
         */
        Channel(const std::string& channelName);

        /**
         * Takes a user and joins them to the set of users in the channel.
         * Will not duplicate users if they are already joined
         * @param user Shared pointer to the user to join the channel
         */
        void join(std::shared_ptr<smak::User>& user);

        /**
         * Takes a shared pointer to a user and has them leave the channel. Will not throw an exception if the user
         * is not in the channel.
         * When a user joins, it will also remove them from the invited users list for the channel.
         * @param user Shared pointer to the user to leave the channel (if they are in the channel).
         */
        void leave(std::shared_ptr<smak::User>& user);

        /**
         * Gives back a set containing all users in the channel
         * @return Set of all users in the channel
         */
        const std::set<std::shared_ptr<smak::User>>& getUsers();

        /**
         * Sends a message to all users in the channel
         * @param message the message to be sent to all users
         */
        void const sendMessage(const std::string& message);

        /**
         * Will make it so that only invited users can join the channel if true.
         * If false, any user can join at any point.
         * @param inviteOnly true if invite only, false if open
         */
        void setInviteOnly(bool inviteOnly);

        /**
         * Invite a user to the channel by adding them to the invited users list for the channel.
         * If the channel is invite only, a user must be a member of the invited users set
         * @param user The user to be invited
         */
        void inviteUser(std::shared_ptr<smak::User> user);

        /**
         * Gives back a set of all invited users.
         * @return a set of shared pointers to users representing the list of users in the invite list
         */
        std::set<std::shared_ptr<smak::User>> getInvitedUsers();

        /**
         * Tells the caller if the channel is invite only
         * @return True if invite only, false if open
         */
        bool isInviteOnly();

        /**
         * Returns the name of the channel
         * @return std::string representing the channel name
         */
        std::string getName();

        /**
         * Sets the topic of the channel to the one passed.
         * If no topic is set, it's left as the default.
         * @param passed The topic the channel should be set to
         */
        void  setTopic(const std::string& passed);

        /**
         * gives back the topic of the channel that is either defaulted or set in the setTopic function.
         * @return std::string representing the topic
         */
        std::string getTopic();

    private:
        // set of all the users in the channel
        std::set<std::shared_ptr<smak::User>> userList;
        // set of all users that are invited to the channel
        std::set<std::shared_ptr<smak::User>> invitedUsers;
        std::string channelName, topic;
        // true if channel is invite only, false if open
        bool inviteOnly;

    };
}



#endif //SMAK_CHANNEL_H
