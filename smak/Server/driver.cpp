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

bool ready = true;

// TODO: Clean up this function
int cclient(std::shared_ptr<cs457::tcpUserSocket> clientSocket,int id, netController netCon)
{
    std::cout << "Waiting for message from Client Thread" << id << std::endl;
    // Just a sanity check to the recently connected clients that they get our star wars meme
    std::thread childT(&cs457::tcpUserSocket::sendString,clientSocket.get(),
                  "Army or not, you must realize you are doomed",true);
    childT.join();
    std::string msg;
    ssize_t val;
    bool cont =true ;
    while (cont)
    {

        tie(msg,val) = clientSocket.get()->recvString();
        std::cout << "Server Received[" << msg <<"]\n";

        // Only the client who sent the message will get a response from General Grievous
        std::thread childT1(&cs457::tcpUserSocket::sendString,clientSocket.get(),"General konobi",true);
        childT1.join();

        // Normally this would process our message, right now it just adds it to a log and sends it to everyone
        netCon.interpret(msg);
        if (msg.substr(0,4) == "EXIT"){
            cont = false;
        }

        if (msg.substr(0,12) == "SERVER_CLOSE") // TODO: I don't know what this does, but it doesn't work
        {
            std::thread childTExit(&cs457::tcpUserSocket::sendString,clientSocket.get(),"GOODBYE EVERYONE",false);
            std::thread childTExit2(&cs457::tcpUserSocket::sendString,clientSocket.get(),"\n",false);
            ready = false;
            cont = false;
            childTExit.join();
            childTExit2.join();
        }
    }
    netCon.closeConnection(clientSocket);
    std::cout<<"Server disconnected from client: " << id << "\n";
    return 1;
}





int driver::driverMain(int argc, char **argv)
{



    // TODO: Check arg count (or just parse the args and forget it)
    std::cout << "Initializing Socket" << std::endl;
    cs457::tcpServerSocket mysocket(atoi(argv[1])); //Set up a TCP socket on port 2000 (FOR SERVER)
    std::cout << "Binding Socket" << std::endl;
    // TODO: Err check. Use main::Error
    mysocket.bindSocket();  //Bind the created SERVER socket "mysocket"
    std::cout << "Listening Socket" << std::endl;
    // TODO: Err check. Use main::Error
    mysocket.listenSocket();  //Listen for incoming client connections
    std::cout << "Waiting to Accept Socket" << std::endl;
    int id = 0;


    std::vector<std::unique_ptr<std::thread>> threadList; //keep track of all the client threads
    srvState serverState = srvState(&threadList);
    netController netCon = netController(&serverState); //netCon now has a server object to manipulate its data

    while (ready) //Creating multiple client sockets to interact with the server
    {

        // TODO: Err check for val<0 Use main::Error
        std::shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val;

        // tuple of socket and its FD
        // TODO: Err check. Use main::Error
        tie(clientSocket,val) = mysocket.acceptSocket();
        // TODO Cleanup error statements
        std::cout << "value for accept is " << val << std::endl;
        std::cout << "Socket Accepted" << std::endl;

        serverState.pushBackSession(clientSocket);
        std::unique_ptr<std::thread> t = std::make_unique<std::thread>(cclient,clientSocket,id, netCon);
        threadList.push_back(std::move(t));

        id++; //not the best way to go about it.
    }
    std::cout << "Broke out of listen for connection loop.\n";
    // TODO: Make this use the thread list inside serverstate.
    for (auto& t: threadList)
    {
        t.get()->join();   //joining all of the active sockets in threadlist vector together
    }
    std::cout<<"Joined all threads\n";


    std::cout << "Server is shutting down after one client\n";
    return 0;
}

