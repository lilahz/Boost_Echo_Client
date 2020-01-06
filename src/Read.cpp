//
// Created by lilachzi@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <User.h>
#include "../include/Read.h"

using namespace std;

Read::Read(ConnectionHandler &connectionHandler) : connectionHandler(connectionHandler) {}

Read::~Read() {

}

void Read::setUser(User *newUser){
    user = newUser;
}

void Read::run() {
    while (!connectionHandler.isActive()){
        std::string line;
        connectionHandler.getLine(line);
        react(line);
    }
}

void Read::react(std::string line) {
    int i = 1;
    std::vector<string> frame;
    while (i != 0) {
        i = line.find_first_of('\n');
        frame.push_back(line.substr(0, i));
        line = line.substr(i, line.length());
    }
    if (frame.at(0) == "CONNECTED") {
        cout << "Login successful" << endl;
    }
    if (frame.at(0) == "RECEIPT") {
        string receiptId = frame.at(1);
        int i = receiptId.find_first_of(':');
        cout << user->getReceipt(receiptId.substr(i, receiptId.size())) << endl;
    }
    if (frame.at(0) == "MESSAGE") {
        string genre = frame.at(3);
        int i = genre.find_first_of(":");
        genre = genre.substr(i, genre.size());
        string msg = frame.at(5);
        // If we received borrow message
        // TODO: add if condition to check if it really a borrow message
        int k = msg.find_last_of(" ");
        string bookName = msg.substr(k, msg.size());
        if (user->bookExist(bookName)) {
            // TODO: send frame - {username} has {bookName}
        }
        // If we received status message
        // TODO: add if condition to check if it really a status message


    }
    if (frame.at(0) == "ERROR") {
        std::string errorMsg = frame.at(1);
        int i = errorMsg.find_first_of(':');
        cout << errorMsg.substr(i, errorMsg.size()) << endl;
    }


}
