#include <User.h>
#include "../include/Read.h"

using namespace std;

Read::Read(ConnectionHandler &connectionHandler) : connectionHandler(connectionHandler) {}

Read::~Read() {

}

void Read::setUser(User *newUser){
    user = newUser;
}

void Read::operator()() {
    while (connectionHandler.isActive()){
        std::string line;
        connectionHandler.getLine(line);
        react(line);
    }
}

void Read::react(std::string line) {
    int i = 1;
    std::vector<string> frame;
    while (i != -1) {
        i = line.find_first_of('\n');
        frame.push_back(line.substr(0, i));
        line = line.substr(i+1, line.length());
    }
    if (frame.at(0) ==  "CONNECTED") {
        cout << "Login successful" << endl;
    }
    if (frame.at(0) == "RECEIPT") {
        string receiptId = frame.at(1);
        int i = receiptId.find_first_of(':');
        string receiptIDString = receiptId.substr(i+1 , receiptId.size());
        cout << user->getReceipt(receiptIDString) << endl;
        //TODO: Disconnect
    }
    if (frame.at(0) == "MESSAGE") {
        string genre = frame.at(3);
        int i = genre.find_first_of(":");
        genre = genre.substr(i, genre.size());
        string subscription = frame.at(1);
        int j = subscription.find_first_of(":");
        subscription = subscription.substr(j, subscription.size());
        // TODO: make sure its in the 5th place
        string msg = frame.at(5);
        messageReact(subscription, genre, msg);
    }
    if (frame.at(0) == "ERROR") {
        std::string errorMsg = frame.at(1);
        int i = errorMsg.find_first_of(':');
        cout << errorMsg.substr(i, errorMsg.size()) << endl;
        connectionHandler.close(); //TODO : do we need to close? what should happen to the user?
    }

}

void Read::messageReact(string subscription, string genre, string message) {
    std::vector<string> msg;
    int i = 1;
    while (i != 0) {
        i = message.find_first_of(" ");
        msg.push_back(message.substr(0, i));
        message = message.substr(i+1, message.length());
    }
    // Borrow flow
    if (msg.at(3) == "borrow") {
        string bookName = msg.at(4);
        if (user->bookExist(genre, bookName)) {
            string frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + "has " + bookName
                    + "\n" + '\u0000';
            connectionHandler.sendLine(frame);
        }
    }
    // TODO: check if possible to remove userName from cond
    if ((msg.at(1) == "has") & (msg.at(0) != user->getName()) & (subscription == user->getSubscriptionId(genre))) {
        string bookName = msg.at(2);
        string borrowFrom = msg.at(0);
        user->addBook(genre, bookName);
        user->addBorrow(bookName, borrowFrom);
        string frame = "SEND\ndestination:" + genre + "\n\n" + "Taking " + bookName + " from " +
                borrowFrom + "\n" + '\u0000';
        connectionHandler.sendLine(frame);
    }
    if ((msg.at(0) == "taking") & (msg.at(msg.size()-1) == user->getName())) {
        string bookName = msg.at(1);
        string borrowFrom = msg.at(3);
        user->removeBook(genre, bookName);
    }
    if ((msg.at(0) == "Returning") & (msg.at(msg.size()-1) == user->getName())) {
        string bookName = msg.at(1);
        user->addBook(genre, bookName);
    }
    // Status flow
    if (msg.at(1) == "status") {
        string frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + ":";
        vector<string> books = user->getBooks(genre);
        for (auto book: books) {
            frame = frame + book + ",";
        }
        frame = frame + "\n" + '\u0000';
        connectionHandler.sendLine(frame);
    }
}
