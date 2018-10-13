//
// Created by michael on 10/2/18.
//

#ifndef SMAK_SERVER_H
#define SMAK_SERVER_H


class Server {
public:
    static void error(const char *msg);
    static int serverMain(int argc, char** argv);
};


#endif //SMAK_SERVER_H
