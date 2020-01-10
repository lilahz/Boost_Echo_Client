#include <mutex>
#include <condition_variable>
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
    std::mutex& mutex;
    std::condition_variable& conditionVariable;
public:
    Write(ConnectionHandler &connectionHandler, std::mutex& mutex, std::condition_variable& conditionVariable);
    virtual ~Write();
    void operator()();
    string createFrame(string inputCommand);
    void  setUser(User *newUser);
};
