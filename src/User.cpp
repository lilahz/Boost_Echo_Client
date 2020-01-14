//
// Created by lilachzi@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <iostream>
#include "User.h"

using namespace std;

User::User(string userName, string password): userName(userName), password(password), active(), books(), borrowFrom(),
    subscriptions(), receipts(), wishBooks() {}

User::~User() {
    books.clear();
    borrowFrom.clear();
    subscriptions.clear();
    receipts.clear();
    wishBooks.clear();
}

void User::addBook(string genre, string bookName) {
    if (books.find(genre) == books.end()) {
        pair<string, vector<string>> tempPair;
        tempPair.first = genre;
        tempPair.second = vector<string>();
        books.insert(tempPair);
    }
    books.at(genre).push_back(bookName);
}

void User::removeBook(string genre, string bookName) {
    for (unsigned int i = 0; i < books.at(genre).size(); i++) {
        if (books.at(genre).at(i) == bookName) {
            books.at(genre).erase(books.at(genre).begin() + i);
        }
    }
}

bool User::bookExist(string genre, string bookName) {
    for (unsigned int i = 0; i < books.at(genre).size(); i++) {
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

bool User::findInBorrow(string bookName) {
    for (unsigned int i = 0; i < borrowFrom.size(); i++) {
        if (books.find(bookName) != books.end()) {
            return true;
        }
    }
    return false;
}

void User::addSubscription(string subsId, string genre) {
    subscriptions.insert(std::pair<string, string>(subsId, genre));
    pair<string, vector<string>> tempPair;
    tempPair.first = genre;
    tempPair.second = vector<string>();
    books.insert(tempPair);
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

    return "";
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

vector<string>* User::getBooks(string genre) {
    return &books.at(genre);

}

void User::addToWishList(string bookName) {
    wishBooks.push_back(bookName);
}

bool User::findInWishList(string bookName) {
    for (auto book : wishBooks){
        if (book == bookName)
            return true;
    }
    return false;
}

void User::removeFromWishList(string bookName) {
    for (unsigned int i = 0; i < wishBooks.size(); i++) {
        if (wishBooks.at(i) == bookName) {
            wishBooks.erase (wishBooks.begin() + i);
        }
    }
}

bool User::isActive() {
    return active;
}

void User::setActive(bool active) {
    this->active = active;
}










