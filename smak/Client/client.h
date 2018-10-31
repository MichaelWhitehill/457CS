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
     * @param disconnect A pointer to an int that will be shared with write, indicating when it's time to close the socket
     */
    static void listenAndPrint(int sockFd, int* disconnect);
    /**
     * Listens to keyboard input and writes to server file descriptor. Meant to be called as a thread
     * @param sockFd Socket file descriptor
     * @param disconnect A pointer to an int that will be shared with listenAndPrint, indicating when it's time to close the socket
     */
    static void writeSock(int sockFd, const int* disconnect);

    static void parseArgsCmdLine(int argc, char *argv[]);

    static std::vector<std::string> split(std::string const &input);

    static bool fileExists(std::string &Filename);

    static std::string getTime();

    static void initialize();

        struct clientInfo{};
};


#endif //SMAK_CLIENT_H
