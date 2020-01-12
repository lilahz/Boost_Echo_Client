#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H



using namespace std;

class User {
private:
    std::string userName;
    std::string password;
    bool active;
    std::map<string, vector<string>> books; // <genre, <books>>
    std::map<string, string> borrowFrom; // <bookName, userName to return book to>
    std::map<string, string> subscriptions; // <subscriptionId, genre>
    std::map<string, string> receipts; // <receiptID, message>
    std::vector<string> wishBooks;

public:
    User(string userName, string password);
    ~User();
    void addBook(string genre, string bookName);
    void removeBook(string genre, string bookName);
    bool bookExist(string genre, string bookName);
    void addBorrow(string bookName, string userName);
    void removeBorrow(string bookName, string userName);
    bool findInBorrow(string bookName);
    void addSubscription(string subsId, string genre);
    void removeSubscription(string subsId);
    string getSubscriptionId(string genre);
    string getName();
    string getLoanerName(string bookName);
    void addReceipt(string receiptId, string message);
    string getReceipt(string receiptId);
    vector<string>* getBooks(string genre);
    void addToWishList(string bookName);
    bool findInWishList(string bookName);
    void removeFromWishList(string bookName);
    void setActive(bool active);
    bool isActive();
};

#endif