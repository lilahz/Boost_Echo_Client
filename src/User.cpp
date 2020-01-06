//
// Created by lilachzi@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include "User.h"

using namespace std;

User::User(string userName, string password): userName(userName), password(password), books(), borrowFrom(), subscriptions() {

}

User::~User() {

}

void User::addBook(string bookName) {
    books.push_back(bookName);
}

void User::removeBook(string bookName) {
    // TODO: maybe can't iterate on map with numbers
    for (int i = 0 ; i < books.size() ; i++) {
        if (books.at(i) == bookName) {
            books.erase(books.begin() + i);
        }
    }
}

bool User::bookExist(string bookName) {
    for (auto book: books) {
        if (book == bookName) {
            return true;
        }
    }
    return false;
}

void User::addBorrow(string bookName, string userName) {
    borrowFrom.insert(bookName,userName);
}

void User::removeBorrow(string bookName, string userName) {
    borrowFrom.erase(bookName);
}

void User::addSubscription(string subsId, string genre) {
    subscriptions.insert(subsId, genre);
}

void User::removeSubscription(string subsId){
    subscriptions.erase(subsId);
}

string User::getSubscriptionId(string genre) {
    for (auto id: subscriptions) {
        if (id.second == genre) {
            return id.first;
        }
    }

    return nullptr;
}

string User::getName() {
    return userName;
}

string User::getLoanerName(string bookName) {
    return borrowFrom.at(bookName);
}

void User::addReceipt(string receiptId, string message) {
    receipts.insert(receiptId, message);
}

string User::getReceipt(string receiptId) {
    return receipts.at(receiptId);
}








