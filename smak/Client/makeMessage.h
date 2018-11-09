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

public : static std::string INITIAL_SETTINGS(std::string name, std::string password, std::string level);

public : static std::string PARSE(std::string passedOP); //To be used for all single command JSON OP's

public : static std::string MSG();

public : static std::string AWAY();

public : static std::string INVITE();

public : static std::string JOIN();

public : static std::string KICK();

public : static std::string KILL();

public : static std::string KNOCK();

public : static std::string NOTICE();

public : static std::string PART();

public : static std::string OPER();

public : static std::string PASS();

public : static std::string PRIVMSG();

public : static std::string SETNAME();

public : static std::string TOPIC();

public : static std::string WALLOPS();

public : static std::string WHO();

public : static std::string LOCK();

public : static std::string UNLOCK();

public : static std::string ISON();

public : static std::string SILENCE();


};


#endif //SMAK_MAKEMESSAGE_H
