#include <Write.h>
#include <Read.h>
#include <User.h>
#include <thread>


int main (int argc, char *argv[]){
    std::string waitForLogin;
    std::string line;

    do {
        getline(cin, line);
        waitForLogin = line.substr(0, line.find_first_of(" "));
    } while(waitForLogin != "login");

    int i = 1;
    std::vector<string> sentence;
    std::string frame;

    while (i != -1) {
        i = line.find_first_of(' ');
        sentence.push_back(line.substr(0, i));
        line = line.substr(i+1, line.length());
    }

    i= sentence.at(1).find_first_of(':');
    std::string host = sentence.at(1).substr(0,i);
    std::string port = sentence.at(1).substr(i+1,sentence.at(1).size());
    std::string userName = sentence.at(2);
    std::string password = sentence.at(3);
    frame = "CONNECT\naccept-version:1.2\nhost:" + host +
            "\nlogin:" + userName +"\npasscode:"+ password + "\n\n" + '\u0000';
    ConnectionHandler connectionHandler1(host, stoi(port));
    cout << frame << endl;
    connectionHandler1.connect();
    User* newUser = new User(userName, password);

    connectionHandler1.sendLine(frame);

    Write threadWrite( connectionHandler1);
    Read threadRead( connectionHandler1);
    threadWrite.setUser(newUser);
    threadRead.setUser(newUser);
    std::thread thWrite((std::thread(threadWrite)));
    std::thread thRead((std::thread(threadRead)));

    thRead.join();
    thWrite.join();

    return 0;
};
