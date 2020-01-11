#include <User.h>
#include <condition_variable>
#include "../include/Read.h"

using namespace std;

Read::Read(ConnectionHandler &connectionHandler, std::mutex& mutex, std::condition_variable& conditionVariable) :
    connectionHandler(connectionHandler), mutex(mutex), conditionVariable(conditionVariable) {}

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

        // Release lock since we connected successfully
        std::unique_lock<std::mutex> uniqueLock(mutex);
        conditionVariable.notify_all();
    }
    if (frame.at(0) == "RECEIPT") {
        string receiptId = frame.at(1);
        int i = receiptId.find_first_of(':');
        string receiptIDString = receiptId.substr(i+1 , receiptId.size());
        cout << user->getReceipt(receiptIDString) << endl;
        if (user->getReceipt(receiptIDString) == "Disconnect") {
            connectionHandler.close();
            // release lock
            std::unique_lock<std::mutex> uniqueLock(mutex);
            conditionVariable.notify_all();
        }
    }
    if (frame.at(0) == "MESSAGE") {
        for (int i = 0 ; i < frame.size(); i++){
           cout << frame.at(i) << endl;
        }
        string genre = frame.at(3);
        int i = genre.find_first_of(":");
        genre = genre.substr(i+1, genre.size());
        string subscription = frame.at(1);
        int j = subscription.find_first_of(":");
        subscription = subscription.substr(j+1, subscription.size());
        string msg = frame.at(5);
        messageReact(genre, msg);
    }
    if (frame.at(0) == "ERROR") {
        std::string errorMsg = frame.at(1);
        int i = errorMsg.find_first_of(':');
        cout << errorMsg.substr(i+2, errorMsg.size()) << endl;
        connectionHandler.close();
        delete(user);

        std::unique_lock<std::mutex> uniqueLock(mutex);
        conditionVariable.notify_all();
    }
}

void Read::messageReact(string genre, string message) {
    std::string originalMessage = message;
    std::vector<string> msg;
    int i = 1;
    while (i != -1) {
        i = message.find_first_of(" ");
        msg.push_back(message.substr(0, i));
        message = message.substr(i+1, message.length());
    }
    // Borrow flow
    if (originalMessage.find("borrow") != -1) {
        std::string bookName;
        for (int i = 4; i < msg.size(); i++) {
            bookName += msg.at(i) + " ";
        }
        bookName = bookName.substr(0, bookName.size()-1);
        if (user->bookExist(genre, bookName)) {
            string frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + " has " + bookName
                           + "\n" + '\u0000';
            connectionHandler.sendLine(frame);
        }
    }
    if (originalMessage.find("has") != -1) {
        std::string bookName;
        for (int i = 2; i < msg.size(); i++) {
            bookName += msg.at(i) + " ";
        }
        bookName = bookName.substr(0, bookName.size()-1);
        if (user->findInWishList(bookName)) {
            string borrowFrom = msg.at(0);
            user->addBook(genre, bookName);
            user->addBorrow(bookName, borrowFrom);
            user->removeFromWishList(bookName);
            string frame = "SEND\ndestination:" + genre + "\n\n" + "Taking " + bookName + " from " +
                           borrowFrom + "\n" + '\u0000';
            connectionHandler.sendLine(frame);
        }
    }
    if ((originalMessage.find("Taking") != -1 ) && (msg.at(msg.size()-1) == user->getName())) {
        int spaceIndex = originalMessage.find_first_of(" ");
        int fromIndex = originalMessage.find("from");
        std::string bookName = originalMessage.substr(spaceIndex+1, fromIndex-spaceIndex-2);
        string borrowFrom = originalMessage.substr(fromIndex+5);
        user->removeBook(genre, bookName);
    }
    if ((msg.at(0) == "Returning") & (msg.at(msg.size()-1) == user->getName())) {
        int spaceIndex = originalMessage.find_first_of(" ");
        int toIndex = originalMessage.find("to");
        std::string bookName = originalMessage.substr(spaceIndex+1, toIndex-spaceIndex-2);
        user->addBook(genre, bookName);
    }
    // Status flow
    if (originalMessage.find("status") != -1) {
        string frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + ":";
        vector<string>* books = user->getBooks(genre);
        // TODO: make the list of books without spaces
        for (auto book: (*books)) {
            frame = frame + book + ",";
        }
        frame = frame + "\n" + '\u0000';
        connectionHandler.sendLine(frame);
    }
}
