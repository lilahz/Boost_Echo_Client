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
    void operator()();
    string createFrame(string inputCommand);
    void  setUser(User *newUser);
};
