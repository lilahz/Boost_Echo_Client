#ifndef BOOST_ECHO_CLIENT_READ_H
#define BOOST_ECHO_CLIENT_READ_H


#include <mutex>
#include "connectionHandler.h"

class Read {

private:
    ConnectionHandler &connectionHandler;
    User *user;
    std::mutex& mutex;
    std::condition_variable& conditionVariable;
public:
    Read(ConnectionHandler &connectionHandler, std::mutex& mutex, condition_variable& conditionVariable);
    Read(const Read &other);
    Read& operator= (const Read &other);
    virtual ~Read();
    void setUser(User *user);
    void operator()();
    void react(std::string line);
    void messageReact(string genre, string message);
};


#endif //BOOST_ECHO_CLIENT_READ_H
