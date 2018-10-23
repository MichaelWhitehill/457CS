//
// Created by michael on 10/4/18.
//

#ifndef SMAK_CLIENT_H
#define SMAK_CLIENT_H
#include <vector>
#include <string>

class client {
public:
    static int clientMain(int argc, char *argv[]);
    /**
     * Busy listens to a file descriptor. Meant to be called as a thread
     * @param sockFd socket file descriptor
     */
    static void listenAndPrint(int sockFd);
    /**
     * Listens to keyboard input and writes to server file descriptor. Meant to be called as a thread
     * @param sockFd Socket file descriptor
     */
    static void writeSock(int sockFd);

    static void parseArgs(int argc, char *argv[]);

    static std::vector<std::string> split(std::string const &input);

    static bool fileExists(std::string &Filename);


        struct clientInfo{};
};


#endif //SMAK_CLIENT_H
