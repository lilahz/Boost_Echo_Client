#include <connectionHandler.h>

#include <Write.h>
#include <condition_variable>

using namespace std;

Write::Write(ConnectionHandler &connectionHandler, std::mutex& mutex, std::condition_variable& conditionVariable) :
    connectionHandler(connectionHandler), mutex(mutex), conditionVariable(conditionVariable), version("1.2"), user(),
    subscriptionID(0), receiptId(0) {}


Write::~Write() {

}
void Write::setUser(User *newUser){
    user = newUser;
}

void Write::operator()() {

    // if we received ERROR frame in the first login command
    std::unique_lock<std::mutex> uniqueLock(mutex);
    conditionVariable.wait(uniqueLock);

    while (connectionHandler.isActive()) {
        std::string line;
        getline(cin, line);
        std::string frame = createFrame(line);
        if (frame != "")
            connectionHandler.sendLine(frame);
        // if line contains logout, wait until receipt is received
        // when it received the wait will finish and we will exit the loop
        // lock
        if (line.find("logout") != -1) {
            std::unique_lock<std::mutex> uniqueLock(mutex);
            conditionVariable.wait(uniqueLock);
        }
    }
}

std::string Write::createFrame(std::string inputCommand){
        int i = 1;
        std::vector<string> sentence;
        std::string frame;
        while (i != -1) {
            i = inputCommand.find_first_of(' ');
            sentence.push_back(inputCommand.substr(0, i));
            inputCommand = inputCommand.substr(i+1, inputCommand.length());
        }
        if (sentence.at(0) == "login"){
            std::string host = sentence.at(1).substr(0,i);
            std::string port = sentence.at(1).substr(i+1,sentence.at(1).size());
            std::string userName = sentence.at(2);
            std::string password = sentence.at(3);
            frame = "CONNECT\naccept-version:1.2\nhost:" + host +
                    "\nlogin:" + userName +"\npasscode:"+ password + "\n\n" + '\u0000';
        }

        else if (sentence.at(0)  == "join") {
            std::string genre = sentence.at(1);
            std::string subscriptionIDString = to_string(subscriptionID);
            frame = "SUBSCRIBE\ndestination:" + genre + "\nid:" + subscriptionIDString +
                    "\nreceipt:" + to_string(receiptId) + "\n\n" + '\u0000';
            user->addSubscription(subscriptionIDString, genre);
            user->addReceipt(to_string(receiptId), "joined club " + genre);
            subscriptionID++ , receiptId++;
        }
        else if (sentence.at(0) == "exit") {
            std::string genre = sentence.at(1);
            std::string subscriptionIDString = user->getSubscriptionId(genre);
            if (subscriptionIDString != "") {
                frame = "UNSUBSCRIBE\nid:" + subscriptionIDString + "\nreceipt-id:" +
                        to_string(receiptId) + "\n\n" + '\u0000';
                user->removeSubscription(subscriptionIDString);
                user->addReceipt(to_string(receiptId), "Exited club " + genre);
                receiptId++;
            }
        }
        else if (sentence.at(0)  == "add") {
            std::string genre = sentence.at(1);
            std::string bookName;
            for (int i = 2; i < sentence.size(); i++) {
                bookName += sentence.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.size()-1);
            frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + " has added the book "
                    + bookName + "\n" + '\u0000';
            user->addBook(genre, bookName);
        }
        else if (sentence.at(0)  == "borrow") {
            std::string genre = sentence.at(1);
            std::string bookName;
            for (int i = 2; i < sentence.size(); i++) {
                bookName += sentence.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.size()-1);
            frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + " wish to borrow "
                    + bookName + "\n" + '\u0000';
            user->addToWishList(bookName);
        }
        else if (sentence.at(0)  == "return") {
            std::string genre = sentence.at(1);
            std::string bookName;
            for (int i = 2; i < sentence.size(); i++) {
                bookName += sentence.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.size()-1);
            if (user->bookExist(genre, bookName)) {
                frame = "SEND\ndestination:" + genre + "\n\nReturning " + bookName + " to " +
                        user->getLoanerName(bookName) + "\n" + '\u0000';
                user->removeBook(genre, bookName);
                user->removeBorrow(bookName, user->getLoanerName(bookName));
            }
        }
        else if (sentence.at(0)  == "status") {
            std::string genre = sentence.at(1);
            frame = "SEND\ndestination:" + genre + "\n\n" + "book status"
              + "\n" + '\u0000';
        }
        else if (sentence.at(0)  == "logout") {
            frame = "DISCONNECT\nreceipt:" + to_string(receiptId) + "\n\n" + '\u0000';
            user->addReceipt(to_string(receiptId),"Disconnect");
        }
        return frame;
    }
