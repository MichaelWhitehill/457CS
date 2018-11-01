//
// Created by michael on 10/16/18.
//

#include <iostream>
#include "srvState.h"

struct serverInfo{
    std::string ServerName, banUsersPath, userLogPath, channelLogPath, bannerPath, startTime;
    std::ofstream banUsersFile, userLogFile, channelLogFile;
    int port =0;
}serverState;

static std::vector<std::unique_ptr<std::thread>> threadList;

auto t = time(NULL);




//Constructor:
smak::srvState::srvState(std::vector<std::unique_ptr<std::thread>> *threadList_p, std::string confPath) {
    this->threadList_p = threadList_p;

    if(fileExists(confPath)&&!confPath.empty()){
        //server config file is good and can be be opened to read

        std::cout << "[Initializing Server (srvState) from .config file]"<<std::endl;

        serverState.startTime = getTime(); //populating start time for INFO JSON call
        std::ifstream read = std::ifstream(confPath);

        if(!read.is_open()){
            throw std::string("ERROR: cannot open config File");
            exit(1);}


        for(std::string line; std::getline(read, line);) {
            std::vector<std::string> parse = split(line);
            if(parse[0]=="-s"){serverState.ServerName = parse[1];}
            else if(parse[0]=="-b"){serverState.banUsersPath = parse[1];}
            else if(parse[0]=="-u"){serverState.userLogPath = parse[1];}
            else if(parse[0]=="-c"){serverState.channelLogPath = parse[1];}
            else if(parse[0]=="-p"){serverState.port = stoi(parse[1]);}
            //need to implement banner file path if we want to do it for extra credit to display at login
            else{throw std::string ("ERROR: Incorrect args in server .conf file\n"
                                    "Server args are in the form -s ServerName -p port# -b banned_users_file_path -u users_log_path -c channel_log_path");}
        }

        std::string uname = "[BANNED_USERS]_";
        uname.append(serverState.ServerName);
        serverState.banUsersPath.append(uname);

        uname = "[USERS]_";
        uname.append(serverState.ServerName);
        serverState.userLogPath.append(uname);

        uname = "[CHANNELS]_";
        uname.append(serverState.ServerName);
        serverState.channelLogPath.append(uname);

        //open files for writing by this serveer object:
        banFileOpen();
        userLogFileOpen();
        channelLogFileOpen();

    }

}



void smak::srvState::pushBackSession(std::shared_ptr<User> session) {
    users.push_back(session);
}


void smak::srvState::appendToChat(const std::string &str) {
    chatLog += str;
}

const std::string& smak::srvState::getChatLog() {
    return chatLog;
}


std::vector<std::shared_ptr<smak::User>> smak::srvState::getUsers() {
    return users;
}

void smak::srvState::removeUser(std::shared_ptr<User> user) {
    for (size_t i = 0; i < users.size(); ++i){
        if(user == users[i]){
            users.erase(users.begin() + i);
        }
    }
}

void smak::srvState::addChannel(std::shared_ptr<smak::Channel> channel) {
    channelList.push_back(channel);
}

std::vector<std::shared_ptr<smak::Channel>> smak::srvState::getChannels() {
    return channelList;
}


std::string smak::srvState::getTime() {
    t = time(NULL);
    struct tm* curtime = localtime(&t);
    std::string ctime = asctime(curtime);
    ctime.erase(std::remove(ctime.begin(),ctime.end(),'\n'),ctime.end());

    return ctime;
}

bool smak::srvState::fileExists(std::string &Filename) {
    std::ifstream file(Filename);
    if (!file)
        return false;
    else
        return true;
}

std::vector<std::string> smak::srvState::split(std::string const &input) {
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());

    return ret;
}

void smak::srvState::banFileOpen() {
    if(!fileExists(serverState.banUsersPath)){
        serverState.banUsersFile = std::ofstream(serverState.banUsersPath);
        if(!serverState.banUsersFile.is_open()){throw std::string("ERROR: Unable to open log File for client");}
        else        std::cout<< "New Banned_Users File opened for writing\n" << std::endl;

        serverState.banUsersFile << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                                 <<"Server Name: " << serverState.ServerName << std::endl
                                 << "Server Port: " << serverState.port << "\n\n" << std::endl;


    }else {

        const char* remv = serverState.banUsersPath.c_str();
        if(std::remove(remv)!=0){throw std::string ("ERROR deleting existing Banned_Users logFile for client");}
        else {
            std::cout << "Old Server Banned_Users log File successfully deleted - opening fresh file" << std::endl;
            serverState.banUsersFile = std::ofstream(serverState.banUsersPath);

            serverState.banUsersFile << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                                     <<"Server Name: " << serverState.ServerName << std::endl
                                     << "Server Port: " << serverState.port<< "\n\n" << std::endl;
        }
    }
}

void smak::srvState::userLogFileOpen() {
    if(!fileExists(serverState.userLogPath)){
        serverState.userLogFile = std::ofstream(serverState.userLogPath);
        if(!serverState.userLogFile.is_open()){throw std::string("ERROR: Unable to open user(s) log File for client");}
        else        std::cout<< "New User(s)_Log File opened for writing\n" << std::endl;

        serverState.userLogFile << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                                 <<"Server Name: " << serverState.ServerName << std::endl
                                 << "Server Port: " << serverState.port << "\n\n" <<std::endl;


    }else {

        const char* remv = serverState.userLogPath.c_str();
        if(std::remove(remv)!=0){throw std::string ("ERROR deleting existing logFile for client");}
        else {
            std::cout << "Old Server User(s) log File successfully deleted - opening fresh file" << std::endl;
            serverState.userLogFile = std::ofstream(serverState.userLogPath);

            serverState.userLogFile << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                                     <<"Server Name: " << serverState.ServerName << std::endl
                                     << "Server Port: " << serverState.port<< "\n\n" << std::endl;
        }
    }
}

void smak::srvState::channelLogFileOpen() {
    if(!fileExists(serverState.channelLogPath)){
        serverState.channelLogFile = std::ofstream(serverState.channelLogPath);
        if(!serverState.channelLogFile.is_open()){throw std::string("ERROR: Unable to open Channel(s) log File for client");}
        else        std::cout<< "New Channel(s)_Log File opened for writing\n" << std::endl;

        serverState.channelLogFile << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                                <<"Server Name: " << serverState.ServerName << std::endl
                                << "Server Port: " << serverState.port << "\n\n" <<std::endl;


    }else {

        const char* remv = serverState.channelLogPath.c_str();
        if(std::remove(remv)!=0){throw std::string ("ERROR deleting existing logFile for client");}
        else {
            std::cout << "Old Server Channel(s) log File successfully deleted - opening fresh file" << std::endl;
            serverState.channelLogFile = std::ofstream(serverState.channelLogPath);

            serverState.channelLogFile << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                                    <<"Server Name: " << serverState.ServerName << std::endl
                                    << "Server Port: " << serverState.port<< "\n\n" << std::endl;
        }
    }
}

void smak::srvState::closeFiles() {
    serverState.banUsersFile.close();
    serverState.userLogFile.close();
    serverState.channelLogFile.close();


}

int smak::srvState::getPort() {
    return serverState.port;
}

std::string smak::srvState::getName() {
    return serverState.ServerName;
}

std::string smak::srvState::getStartTime() {
    return serverState.startTime;
}




