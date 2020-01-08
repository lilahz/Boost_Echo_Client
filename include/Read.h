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
    void operator()();
    void react(std::string line);
    void messageReact(string subscription, string genre, string message);
};


#endif //BOOST_ECHO_CLIENT_READ_H
