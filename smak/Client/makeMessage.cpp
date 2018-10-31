//
// Created by jonathan on 10/25/18.
//

#include "makeMessage.h"
using namespace rapidjson;

//#define for each JSON string in utils.h
//Test JOIN MSG SETNAME
std::string makeMessage::MSG() {

    std::string json = "{\"OP\":\"MSG\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    std::cout << "Please enter the channel to send MSG: " << std::endl;
    std::string channel;
    std::getline(std::cin, channel);
    std::cout << "Please enter the message to channel: " << channel << " to" << std::endl;
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
    std::cout << "Please enter the AWAY string msg: " << std::endl;
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
    d.AddMember("client", contact, d.GetAllocator());

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
    return std::__cxx11::string();
}

std::string makeMessage::NICK() {
    return std::__cxx11::string();
}

std::string makeMessage::NOTICE() {
    return std::__cxx11::string();
}

std::string makeMessage::PART() {
    return std::__cxx11::string();
}

std::string makeMessage::OPER() {
    return std::__cxx11::string();
}

std::string makeMessage::PASS() {
    return std::__cxx11::string();
}

std::string makeMessage::PRIVMSG() {
    return std::__cxx11::string();
}

std::string makeMessage::QUIT() {
    return std::__cxx11::string();
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

std::string makeMessage::USER() {
    return std::__cxx11::string();
}

std::string makeMessage::USERHOST() {
    return std::__cxx11::string();
}

std::string makeMessage::USERIP() {
    return std::__cxx11::string();
}

std::string makeMessage::USERS() {
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


