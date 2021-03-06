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
#include "User.h"

bool ready = true;

// TODO: Clean up this function
int cclient(const std::shared_ptr<smak::User> &user,int id, smak::netController netCon)
{
    std::cout << "Waiting for message from Client Thread" << id << std::endl;
    std::string msg;
    ssize_t val;
    bool cont =true ;
    while (cont)
    {
        tie(msg,val) = user.get()->getSession().get()->recvString();;
        std::cout << "Server Received[" << msg <<"]\n";

        // Only the client who sent the message will get this response
        user.get()->sendString("_SERVER_: recvd message");

        // This processes the message based on it's JSON content
        netCon.interpret(msg, user);

        if (msg.substr(0,4) == "EXIT"){
            cont = false;
        }
        if (user.get()->disconnect())
            cont = false;

        if (!(netCon.isReady())) // TODO: I don't know what this does, but it doesn't work
        {
            ready = false;
            cont = false;
        }
    }
    netCon.closeUserConnection(user);
    std::cout<<"Server disconnected from client: " << id << "\n";
    return 1;
}

int smak::driver::driverMain(int argc, char **argv)
{

    std::vector<std::unique_ptr<std::thread>> threadList; //keep track of all the client threads
    srvState serverState = srvState(&threadList, argv[1]);   //keep track of the state of this server and its variables
    netController netCon = netController(&serverState); //netCon now has a server object to manipulate its data

    if(argc<1){throw std::string("ERROR: Minimum argument to initiate server is .conf file path");}
    std::cout << "Initializing Socket" << std::endl;
    smak::tcpServerSocket mysocket(serverState.getPort()); //Set up a TCP socket on port 2000 (FOR SERVER)
    if(serverState.getPort() ==0 ){
        throw std::string ("ERROR: Server port was not correctly read from conf files");
    }
    std::cout << "Binding Socket" << std::endl;
    // TODO: Err check. Use main::Error
    mysocket.bindSocket();  //Bind the created SERVER socket "mysocket"
    std::cout << "Listening Socket" << std::endl;
    // TODO: Err check. Use main::Error
    mysocket.listenSocket();  //Listen for incoming client connections
    std::cout << "Waiting to Accept Socket\n" << std::endl;
    int id = 0;

    //TODO: call create function in srvState to create fresh files for the new server

    while (ready) //Creating multiple client sockets to interact with the server
    {

        // TODO: Err check for val<0 Use main::Error
        std::shared_ptr<smak::tcpUserSocket> clientSocket;
        int val;
        // tuple of socket and its FD
        // TODO: Err check. Use main::Error
        tie(clientSocket,val) = mysocket.acceptSocket();
        // TODO Cleanup error statements
        std::cout << "value for accept is " << val << std::endl;
        std::cout << "Socket Accepted" << std::endl;

        std::shared_ptr<User> user = std::make_shared<User>(User(clientSocket));
        serverState.pushBackSession(user);
        std::unique_ptr<std::thread> t = std::make_unique<std::thread>(cclient,user,id, netCon);
        threadList.push_back(std::move(t));

        //TODO: call the write function in srvState to update files on channels, users etc
       // serverState.writeFiles();
       // serverState.closeFiles(); //close all files to avoid memory leak


        id++; //not the best way to go about it.
    }

    std::cout << "Broke out of listen for connection loop.\n";
    // TODO: Make this use the thread list inside serverstate.
    for (auto& t: threadList)
    {
        t.get()->join();   //joining all of the active sockets in threadlist vector together
    }
    std::cout<<"Joined all threads\n";
    //serverState.writeFiles();
    serverState.closeFiles(); //close all files to avoid memory leak

    std::cout << "Server is shutting down after one client\n";
    return 0;
}

