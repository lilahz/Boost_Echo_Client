//
// Created by lilachzi@wincs.cs.bgu.ac.il on 06/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_READ_H
#define BOOST_ECHO_CLIENT_READ_H


#include "connectionHandler.h"

class Read {

private:
    ConnectionHandler &connectionHandler;
    User *user;
public:

    Read(ConnectionHandler &connectionHandler);
    virtual ~Read();
    void setUser(User *user);
    void run();
    void react(std::string line);
};


#endif //BOOST_ECHO_CLIENT_READ_H
