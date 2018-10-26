//
// Created by jonathan on 10/25/18.
//

#include "makeMessage.h"
using namespace rapidjson;

std::string makeMessage::test (){

    std::string json = "{\"OP\":\"SETNAME\",\"Name\":\"Mr.Shrek\"}";
    rapidjson::Document d;
    d.Parse(json.c_str());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();


}