//
// Created by jonathan on 10/25/18.
//

#include "makeMessage.h"
using namespace rapidjson;

//#define for each JSON string in utils.h
//Test JOIN MSG SETNAME


std::string makeMessage::INITIAL_SETTINGS(std::string name, std::string password, std::string level) {

    std::string json = "{\"OP\":\"USER\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    rapidjson::Value n;
    n = StringRef(name.c_str());
    d.AddMember("name", n, d.GetAllocator());

    rapidjson::Value p;
    p = StringRef(password.c_str());
    d.AddMember("password", p, d.GetAllocator());

    rapidjson::Value l;
    l = StringRef(level.c_str());
    d.AddMember("level", l, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();

}


std::string makeMessage::MSG() {

    std::string json = "{\"OP\":\"MSG\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the channel to send MSG: " << std::endl;
    std::string channel;
    std::getline(std::cin, channel);
    std::cout << "Please enter the message to channel: " << channel << std::endl;
    std::string msg;
    std::getline(std::cin, msg);

    rapidjson::Value contact;
    contact = StringRef(channel.c_str());
    d.AddMember("channel", contact, d.GetAllocator());

    rapidjson::Value val;
    val = StringRef(msg.c_str());
    d.AddMember("message", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();

}

std::string makeMessage::AWAY() {
    std::string json = "{\"OP\":\"AWAY\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::string input;
    std::cout << "Please enter the AWAY string msg: \n (enter 'here' to clear AWAY status)" << std::endl;
    std::getline(std::cin, input);

    rapidjson::Value contact;
    contact = StringRef(input.c_str());
    d.AddMember("message", contact, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::INVITE() {

    std::string json = "{\"OP\":\"INVITE\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the nickname to INVITE: " << std::endl;
    std::string nick;
    std::getline(std::cin, nick);
    std::cout << "Please enter the channel name to INVITE: " << nick << " to" << std::endl;
    std::string channel;
    std::getline(std::cin, channel);

    rapidjson::Value contact;
    contact = StringRef(nick.c_str());
    d.AddMember("nickname", contact, d.GetAllocator());

    rapidjson::Value val;
    val = StringRef(channel.c_str());
    d.AddMember("channel", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::JOIN() {

    std::string json = "{\"OP\":\"JOIN\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the channel to JOIN: " << std::endl;
    std::string channel;
    std::getline(std::cin, channel);

    rapidjson::Value val;
    val = StringRef(channel.c_str());
    d.AddMember("channel", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::KICK() {

    std::string json = "{\"OP\":\"KICK\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the client to KICK: " << std::endl;
    std::string client;
    std::getline(std::cin, client);
    std::cout << "Please enter the channel name to KICK: " << client << " from" << std::endl;
    std::string channel;
    std::getline(std::cin, channel);


    rapidjson::Value contact;
    contact = StringRef(client.c_str());
    d.AddMember("name", contact, d.GetAllocator());

    rapidjson::Value val;
    val = StringRef(channel.c_str());
    d.AddMember("channel", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();

}

std::string makeMessage::KILL() {
    return std::__cxx11::string();
}

std::string makeMessage::KNOCK() {



}

std::string makeMessage::NICK() {
    //This command necessitates a completely new field nickname vs username/password
    return std::__cxx11::string();
}

std::string makeMessage::NOTICE() {
    return std::__cxx11::string();
}

std::string makeMessage::PART() {
    return std::__cxx11::string();
}

std::string makeMessage::OPER() {
    std::string json = "{\"OP\":\"OPER\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::PASS() {
    return std::__cxx11::string();
}

std::string makeMessage::PRIVMSG() {

    //should be a check for if the recipient is on the channel you are on? or just send to any user anywhere

    std::string json = "{\"OP\":\"PRIVMSG\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the user to send PRIVMSG to: " << std::endl;
    std::string user;
    std::getline(std::cin, user);
    std::cout << "Please enter the private message to user: " << user << std::endl;
    std::string msg;
    std::getline(std::cin, msg);

    rapidjson::Value contact;
    contact = StringRef(user.c_str());
    d.AddMember("name", contact, d.GetAllocator());

    rapidjson::Value val;
    val = StringRef(msg.c_str());
    d.AddMember("message", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}



std::string makeMessage::SETNAME() {
    std::string json = "{\"OP\":\"SETNAME\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::string name;
    std::cout << "Please enter the name you want to change to: " << std::endl;
    std::getline(std::cin, name);

    rapidjson::Value contact;
    contact = StringRef(name.c_str());
    d.AddMember("name", contact, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::TOPIC() {
    return std::__cxx11::string();
}

std::string makeMessage::USER() { //This is implemented through SET method
    return std::__cxx11::string();
}

std::string makeMessage::USERHOST() {
    return std::__cxx11::string();
}

std::string makeMessage::USERIP() {
    return std::__cxx11::string();
}



std::string makeMessage::WALLOPS() {
    return std::__cxx11::string();
}

std::string makeMessage::WHO() {
    return std::__cxx11::string();
}

std::string makeMessage::WHOIS() {
    return std::__cxx11::string();
}

std::string makeMessage::LOCK() {
    std::string json = "{\"OP\":\"LOCK\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the channel name to LOCK: " << std::endl;
    std::string channel;
    std::getline(std::cin, channel);

    rapidjson::Value val;
    val = StringRef(channel.c_str());
    d.AddMember("channel", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::UNLOCK() {
    std::string json = "{\"OP\":\"UNLOCK\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the channel name to UNLOCK: " << std::endl;
    std::string channel;
    std::getline(std::cin, channel);

    rapidjson::Value val;
    val = StringRef(channel.c_str());
    d.AddMember("channel", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}



//ALL THE BELOW METHODS CAN BE PUT ALL INTO ONE METHOD - just pass a JSON string and populate the OP field

//TIME , VERSION, RULES, PING, INFO, USERS, QUIT
std::string makeMessage::PARSE(std::string passedOP) {
    std::string json = "{}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    rapidjson::Value val;
    val = StringRef(passedOP.c_str());
    d.AddMember("OP", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}





