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

int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket,int id, netController netCon)
{
    cout << "Waiting for message from Client Thread" << id << std::endl;
    thread childT(&cs457::tcpUserSocket::sendString,clientSocket.get(),"Army or not, you must realize you are doomed",true);
    childT.join();
    string msg;
    ssize_t val;
    bool cont =true ;
    while (cont)
    {
        tie(msg,val) = clientSocket.get()->recvString();
        if (msg.substr(0,4) == "EXIT")
            cont = false;
        cout << "Server recieved: " << msg <<"| echoing back\n";
//        thread childT1(&cs457::tcpUserSocket::sendString,clientSocket.get(),msg,true);
//        childT1.join();
        thread childT2(&cs457::tcpUserSocket::sendString,clientSocket.get(),"General konobi",true);
        childT2.join();
        netCon.interpret(msg);

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
    vector<unique_ptr<thread>> threadList; //keep track of all the client threads
    srvState serverState = srvState(&threadList);
    netController netCon = netController(&serverState);

    while (ready) //Creating multiple client sockets to interact with the server
    {

        //** Need check for val <0
        shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val;

        // tuple of socket and its FD
        tie(clientSocket,val) = mysocket.acceptSocket();
        cout << "value for accept is " << val << std::endl;
        cout << "Socket Accepted" << std::endl;
        serverState.pushBackSession(clientSocket);
        unique_ptr<thread> t = make_unique<thread>(cclient,clientSocket,id, netCon);
        threadList.push_back(std::move(t));

        id++; //not the best way to go about it.
    }
    for (auto& t: threadList)
    {
        t.get()->join();   //joining all of the active sockets in threadlist vector together
    }
    std::cout<<"Joined all threads\n";


    cout << "Server is shutting down after one client" << endl;
    return 0;
}

