#include <connectionHandler.h>
#include <User.h>
#include <Write.h>

using namespace std;

Write::Write(ConnectionHandler &connectionHandler) : connectionHandler(connectionHandler), version("1.2"), user(),
subscriptionID(1), receiptId(1) {}

Write::~Write() {

}
void Write::setUser(User *newUser){
    user = newUser;
}

void Write::run() {
        // TODO : maybe do while
        while (!connectionHandler.isActive()) {
            std::string line;
            getline(cin, line);
            std::string frame = createFrame(line);
            connectionHandler.sendLine(frame);
        }
    }

std::string Write::createFrame(std::string inputCommand){
        int i = 1;
        std::vector<string> sentence;
        std::string frame;
        while (i != 0) {
            i = inputCommand.find_first_of(' ');
            sentence.push_back(inputCommand.substr(0, i));
            inputCommand = inputCommand.substr(i+1, inputCommand.length());
        }

        if (sentence.at(0) == "login") {
            int i= sentence.at(1).find_first_of(':');
            std::string host = sentence.at(1).substr(0,i);
            std::string port = sentence.at(1).substr(i+1,sentence.at(1).size());
            std::string userName = sentence.at(2);
            std::string password = sentence.at(3);
            frame = "CONNECT\naccept-version:" + version + "\nhost:" + host +
                    "\nlogin:" + userName +"\npasscode:"+ password + "\n\n" + '\u0000';
            ConnectionHandler connectionHandler1(host, stoi(port));
            connectionHandler1.connect();
        }
        if (sentence.at(0)  == "join") {
            std::string genre = sentence.at(1);
            frame = "SUBSCRIBE\ndestination:" + genre + "\nid:" + to_string(subscriptionID) +
                    "\nreceipt:" + to_string(receiptId) + "\n\n" + '\u0000';
            user->addSubscription(to_string(subscriptionID), genre);
            user->addReceipt(to_string(receiptId), "joined club" + genre);
            subscriptionID++ , receiptId++;
        }
        if (sentence.at(0) == "exit") {
            std::string genre = sentence.at(1);
            frame = "UNSUBSCRIBE\nid:" + user->getSubscriptionId(sentence.at(0)) + "\nreceipt-id:" +
                    to_string(receiptId) + "\n\n" + '\u0000';
            user->removeSubscription(user->getSubscriptionId(sentence.at(0)));
            user->addReceipt(to_string(receiptId), "Exited club " + genre);
            receiptId++;
        }
        if (sentence.at(0)  == "add") {
            std::string genre = sentence.at(1);
            std::string bookName = sentence.at(2);
            frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + "has added the book "
                    + bookName + "\n" + '\u0000';
            user->addBook(genre, bookName);
        }
        if (sentence.at(0)  == "borrow") {
            std::string genre = sentence.at(1);
            std::string bookName = sentence.at(2);
            frame = "SEND\ndestination:" + genre + "\n\n" + user->getName() + "wish to borrow "
                    + bookName + "\n" + '\u0000';
        }
        if (sentence.at(0)  == "return") {
            std::string genre = sentence.at(1);
            std::string bookName = sentence.at(2);
            frame = "SEND\ndestination:" + genre + "\n\nReturning " + bookName + " to" +
                    user->getLoanerName(bookName) + "\n" + '\u0000';
            user->removeBook(genre, bookName);
            user->removeBorrow(bookName, user->getLoanerName(bookName));
        }
        if (sentence.at(0)  == "status") {
            std::string genre = sentence.at(1);
            frame = "SEND\ndestination:" + genre + "\n\n" + "book status"
              + "\n" + '\u0000';
        }
        if (sentence.at(0)  == "logout") {
            frame = "DISCONNECT\nreceipt:" + to_string(receiptId) + "\n\n" + '\u0000';
            connectionHandler.close();
        }
        return frame;
    }
