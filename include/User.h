//
// Created by lilachzi@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <string>
#include <map>
#include <vector>

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#endif //BOOST_ECHO_CLIENT_USER_H

using namespace std;

class User {
private:
    std::string userName;
    std::string password;
    std::vector<string> books;
    std::map<string, string> borrowFrom; // <bookName, userName to return book to>
    std::map<string, string> subscriptions; // <subscriptionId, genre>
    std::map<string, string> receipts; // <receiptID, message>
public:
    User(string userName, string password);
    ~User();
    void addBook(string bookName);
    void removeBook(string bookName);
    bool bookExist(string bookName);
    void addBorrow(string bookName, string userName);
    void removeBorrow(string bookName, string userName);
    void addSubscription(string subsId, string genre);
    void removeSubscription(string subsId);
    string getSubscriptionId(string genre);
    string getName();
    string getLoanerName(string bookName);
    void addReceipt(string receiptId, string message);
    string getReceipt(string receiptId);
};

