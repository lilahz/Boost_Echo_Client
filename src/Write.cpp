#include <connectionHandler.h>

#include <Write.h>
#include <condition_variable>

using namespace std;

Write::Write(ConnectionHandler &connectionHandler, std::mutex& mutex, std::condition_variable& conditionVariable) :
    connectionHandler(connectionHandler), mutex(mutex), conditionVariable(conditionVariable), version("1.2"), user(),
    subscriptionID(0), receiptId(0) {}

Write::Write(const Write &other) : connectionHandler(other.connectionHandler), mutex(other.mutex), conditionVariable(other.conditionVariable), version("1.2"), user(other.user),
                                   subscriptionID(0), receiptId(0) {}

Write &Write::operator=(const Write &other) {
    if (this != &other){
        user=other.user;
    }
    return *this;
}

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
        if (signed(line.find("logout")) != (-1)) {
            conditionVariable.wait(uniqueLock);
        }
    }
}

std::string Write::createFrame(std::string inputCommand){
        signed int i = 1;
        std::vector<string> sentence;
        std::string frame;
        while (i != -1) {
            i = inputCommand.find_first_of(' ');
            sentence.push_back(inputCommand.substr(0, i));
            inputCommand = inputCommand.substr(i+1, inputCommand.length());
        }
        if ((sentence.at(0) == "login") & (!user->isActive())){
            std::string host = sentence.at(1).substr(0,i);
            std::string port = sentence.at(1).substr(i+1,sentence.at(1).size());
            std::string userName = sentence.at(2);
            std::string password = sentence.at(3);
            frame = "CONNECT\naccept-version:1.2\nhost:" + host +
                    "\nlogin:" + userName +"\npasscode:"+ password + "\n\n";
        }

        else if (sentence.at(0)  == "join") {
            std::string genre = sentence.at(1);
            std::string subscriptionIDString = to_string(subscriptionID);
            frame = "SUBSCRIBE\ndestination:" + genre + "\nid:" + subscriptionIDString +
                    "\nreceipt:" + to_string(receiptId) + "\n\n";
            user->addSubscription(subscriptionIDString, genre);
            user->addReceipt(to_string(receiptId), "joined club " + genre);
            subscriptionID++ , receiptId++;
        }
        else if (sentence.at(0) == "exit") {
            std::string genre = sentence.at(1);
            std::string subscriptionIDString = user->getSubscriptionId(genre);
            if (subscriptionIDString != "") {
                frame = "UNSUBSCRIBE\nid:" + subscriptionIDString + "\nreceipt-id:" +
                        to_string(receiptId) + "\n\n";
                user->removeSubscription(subscriptionIDString);
                user->addReceipt(to_string(receiptId), "Exited club " + genre);
                receiptId++;
            }
        }
        else if (sentence.at(0)  == "add") {
            std::string genre = sentence.at(1);
            std::string bookName;
            for (unsigned int i = 2; i < sentence.size(); i++) {
                bookName += sentence.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.size()-1);
            frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + " has added the book "
                    + bookName + "\n";
            user->addBook(genre, bookName);
        }
        else if (sentence.at(0)  == "borrow") {
            std::string genre = sentence.at(1);
            std::string bookName;
            for (unsigned int i = 2; i < sentence.size(); i++) {
                bookName += sentence.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.size()-1);
            frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + " wish to borrow "
                    + bookName + "\n";
            user->addToWishList(bookName);
        }
        else if (sentence.at(0)  == "return") {
            std::string genre = sentence.at(1);
            std::string bookName;
            for (unsigned int i = 2; i < sentence.size(); i++) {
                bookName += sentence.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.size()-1);
            if (user->bookExist(genre, bookName) & (user->findInBorrow(bookName))) {
                frame = "SEND\ndestination:" + genre + "\n\nReturning " + bookName + " to " +
                        user->getLoanerName(bookName) + "\n";
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
            frame = "DISCONNECT\nreceipt:" + to_string(receiptId) + "\n\n";
            user->addReceipt(to_string(receiptId),"Disconnect");
        }
        return frame;
    }
