#include <iostream>
#include <string>
#include <tuple>
#include <thread>
#include <vector>
#include <memory>
#include "Socket.h"

using namespace std;

bool ready = true;

int cclient(cs457::tcpUserSocket clientSocket,int id)
{

    cout << "Waiting for message from Client Thread" << id << std::endl;
    string msg;
    ssize_t val;
    bool cont =true;
    while (cont)
    {
        tie(msg,val) = clientSocket.recvString();
        if (msg.substr(0,4) == "EXIT")
            cont = false;
        if (msg.substr(0,6) == "SERVER")
            ready = false;

        cout << "The client is sending message " << msg << " -- With value return = " << val << endl;
        clientSocket.sendString(msg);
        clientSocket.sendString("\n");
        cout << "waiting for another message" << endl;
    }

    clientSocket.sendString("goodbye");
    clientSocket.closeSocket();
    return 1;
}

int main(int argc, char * argv[])
{
    cout << "Initializing Socket" << std::endl;
    cs457::tcpServerSocket mysocket(2000);
    cout << "Binding Socket" << std::endl;
    mysocket.bindSocket();
    cout << "Listening Socket" << std::endl;
    mysocket.listenSocket();
    cout << "Waiting to Accept Socket" << std::endl;
    int id = 0;
    vector<unique_ptr<thread>> threadList;
    while (ready)
    {
        cs457::tcpUserSocket clientSocket;
        int val;
        tie(clientSocket,val) = mysocket.acceptSocket();
        cout << "value for accept is " << val << std::endl;
        cout << "Socket Accepted" << std::endl;
        unique_ptr<thread> t = make_unique<thread>(cclient,clientSocket,id);
        threadList.push_back(std::move(t));
        id++; //not the best way to go about it.
       // threadList.push_back(t);
    }

    for (auto& t: threadList)
    {
        t.get()->join();
    }
    cout << "Server is shutting down after one client" << endl;
    return 0;
}
