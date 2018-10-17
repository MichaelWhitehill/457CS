//
// Created by michael on 10/4/18.
//

#ifndef SMAK_CLIENT_H
#define SMAK_CLIENT_H


class client {
public:
    static int clientMain(int argc, char *argv[]);
    static void listenAndPrint(int sockFd);
    static void writeSock(int sockFd);
};


#endif //SMAK_CLIENT_H
