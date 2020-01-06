//
// Created by lilachzi@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include "connectionHandler.h"
#include "User.h"

#ifndef BOOST_ECHO_CLIENT_WRITE_H
#define BOOST_ECHO_CLIENT_WRITE_H

#endif //BOOST_ECHO_CLIENT_WRITE_H

class Write {
private:
    ConnectionHandler &connectionHandler;
    std::string version;
    User *user;
    int subscriptionID;
    int receiptId;
public:
    Write(ConnectionHandler &connectionHandler);
    virtual ~Write();
    void run();
    std::string createFrame(std::string inputCommand);
    void setUser(User *newUser);
};
