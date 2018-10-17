#include <iostream>
#include <string>
#include <tuple>
#include <thread>
#include <vector>
#include <memory>
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include "driver.h"
#include "srvState.h"
#include "netController.h"

using namespace std;

bool ready = true;

int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket,int id)
{

    cout << "Waiting for message from Client Thread" << id << std::endl;
    string msg;
    ssize_t val;
    bool cont =true ;
    while (cont)
    {
        tie(msg,val) = clientSocket.get()->recvString();
        if (msg.substr(0,4) == "EXIT")
            cont = false;

         cout << "[SERVER] The client is sending message " << msg << " -- With value return = " << val << endl;
        string s =  "[SERVER REPLY] The client is sending message:" + msg  + "\n";


        thread childT1(&cs457::tcpUserSocket::sendString,clientSocket.get(),s,true);

        childT1.join();
        if (msg.substr(0,6) == "SERVER")
        {
            thread childTExit(&cs457::tcpUserSocket::sendString,clientSocket.get(),"GOODBYE EVERYONE",false);
            thread childTExit2(&cs457::tcpUserSocket::sendString,clientSocket.get(),"\n",false);
            ready = false;
            cont = false;
            childTExit.join();
            childTExit2.join();
        }
        else
        {
            cout << "waiting for another message" << endl;
        }
    }

    clientSocket.get()->sendString("goodbye");

    clientSocket.get()->closeSocket();
    return 1;
}

int driver::driverMain(int argc, char **argv)
{
    cout << "Initializing Socket" << std::endl;
    cs457::tcpServerSocket mysocket(atoi(argv[1])); //Set up a TCP socket on port 2000 (FOR SERVER)
    cout << "Binding Socket" << std::endl;
    mysocket.bindSocket();  //Bind the created SERVER socket "mysocket"
    cout << "Listening Socket" << std::endl;
    mysocket.listenSocket();  //Listen for incoming client connections
    cout << "Waiting to Accept Socket" << std::endl;
    int id = 0;

    srvState serverState = srvState();
    netController netCon = netController(serverState);

    while (ready) //Creating multiple client sockets to interact with the server
    {

        //** Need check for val <0
        shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val;

        // tuple of socket and its FD
        tie(clientSocket,val) = mysocket.acceptSocket();
        serverState.pushBackSession(clientSocket);
        cout << "value for accept is " << val << std::endl;
        cout << "Socket Accepted" << std::endl;
        unique_ptr<thread> t = make_unique<thread>(cclient,clientSocket,id);
        serverState.pushBackThread(std::move(t));

        id++; //not the best way to go about it. 
        // threadList.push_back(t);

    }
    auto threadList = serverState.getThreads();
    for (auto& t: threadList)
    {
        t.get()->join();   //joining all of the active sockets in threadlist vector together
    }


    cout << "Server is shutting down after one client" << endl;
    return 0;
}

