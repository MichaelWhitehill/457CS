//
// Created by jonathan on 10/25/18.
//

#include "makeMessage.h"
using namespace rapidjson;

std::string makeMessage::AWAY(std:: string passed) {
    std::string json = "{\"OP\":\"AWAY\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    rapidjson::Value contact;
    contact = StringRef(passed.c_str());
    d.AddMember("message", contact, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::INVITE(std::string nick, std::string channel) {

    std::string json = "{\"OP\":\"INVITE\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

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

std::string makeMessage::JOIN(std::string channel) {

    std::string json = "{\"OP\":\"JOIN\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    rapidjson::Value val;
    val = StringRef(channel.c_str());
    d.AddMember("channel", val, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();
}

std::string makeMessage::KICK(std::string client, std::string channel) {

    std::string json = "{\"OP\":\"KICK\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

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
    return std::__cxx11::string();
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
