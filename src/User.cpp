//
// Created by lilachzi@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <iostream>
#include "User.h"

using namespace std;

User::User(string userName, string password): userName(userName), password(password), books(), borrowFrom(), subscriptions() {

}

User::~User() {

}

void User::addBook(string genre, string bookName) {
    books.at(genre).push_back(bookName);
}

void User::removeBook(string genre, string bookName) {
    // TODO: check if the correct book is erased
    for (int i = 0; i < books.at(genre).size(); i++) {
        if (books.at(genre).at(i) == bookName) {
            books.at(genre).erase(books.at(genre).begin()+i-1);
        }
    }
}

bool User::bookExist(string genre, string bookName) {
    for (int i = 0; i < books.at(genre).size(); i++) {
        if (books.at(genre).at(i) == bookName) {
            return true;
        }
    }
    return false;
}

void User::addBorrow(string bookName, string userName) {
//    borrowFrom.insert(bookName, userName);
    borrowFrom.insert(std::pair<string,string>(bookName, userName));
}

void User::removeBorrow(string bookName, string userName) {
    borrowFrom.erase(bookName);
}

void User::addSubscription(string subsId, string genre) {
    subscriptions.insert(std::pair<string,string>(subsId, genre));
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

    receipts.insert(std::pair<string, string>(receiptId, message));
}

string User::getReceipt(string receiptId) {
    return receipts.at(receiptId);
}

vector<string> User::getBooks(string genre) {
    return books.at(genre);

}

User::User() {}








