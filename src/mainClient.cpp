//
// Created by lilachzi@wincs.cs.bgu.ac.il on 05/01/2020.
//

#include <connectionHandler.h>

using namespace std;

class Write {
private:
    ConnectionHandler &connectionHandler;

public:
    Write(ConnectionHandler &connectionHandler1): connectionHandler(connectionHandler1) {}

    void opeator() {
        while (!connectionHandler.isActive()) {

            std::vector<string> sentence;
            int i=1;
            std::string line;
            getline(cin, line);


            while (i != 0){
                 i = line.find_first_of(' ');
                sentence.push_back(line.substr(0, i));
                line = line.substr(i, line.length());
            }

            if (sentence.at(0) == "login")
            {
                std::string port = sentence.at(1);
                std::string userName = sentence.at(2);
                std::string password = sentence.at(3);

            }
            if (sentence.at(0)  == "join")
            {
                std::string genre = sentence.at(1);
            }
            if (sentence.at(0)  == "add")
            {
                std::string genre = sentence.at(1);
                std::string bookName = sentence.at(2);

            }
            if (sentence.at(0)  == "borrow")
            {
                std::string genre = sentence.at(1);
                std::string bookName = sentence.at(2);
            }
            if (sentence.at(0)  == "return")
            {
                std::string genre = sentence.at(1);
                std::string bookName = sentence.at(2);
            }
            if (sentence.at(0)  == "status")
            {
                std::string genre = sentence.at(1);
            }
            if (sentence.at(0)  == "logout")
            {

            }
        }
    }
};
