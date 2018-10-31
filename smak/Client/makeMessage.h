//
// Created by jonathan on 10/25/18.
//
#include <iostream>
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

#ifndef SMAK_MAKEMESSAGE_H
#define SMAK_MAKEMESSAGE_H

// using namespace rapidjson;

class makeMessage {

public : static std::string AWAY();

public : static std::string INVITE();

public : static std::string JOIN(std::string channel);

public : static std::string KICK(std::string client, std::string channel);

public : static std::string KILL();

public : static std::string KNOCK();

public : static std::string NICK();

public : static std::string NOTICE();

public : static std::string PART();

public : static std::string OPER();

public : static std::string PASS();

public : static std::string PRIVMSG();

public : static std::string QUIT();

public : static std::string SETNAME();

public : static std::string TOPIC();

public : static std::string USER();

public : static std::string USERHOST();

public : static std::string USERIP();

public : static std::string USERS();

public : static std::string WALLOPS();

public : static std::string WHO();

public : static std::string WHOIS();




};


#endif //SMAK_MAKEMESSAGE_H
