CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system
CFLAGS += -pthread
LDFLAGS += -pthread

all: MainClient

MainClient: bin/connectionHandler.o bin/MainClient.o bin/Read.o bin/User.o bin/Write.o
	g++ -o bin/MainClient bin/connectionHandler.o bin/MainClient.o bin/Read.o bin/User.o bin/Write.o $(LDFLAGS)

bin/connectionHandler.o: src/connectionHandler.cpp include/connectionHandler.h
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/MainClient.o: src/MainClient.cpp
	g++ $(CFLAGS) -o bin/MainClient.o src/MainClient.cpp

bin/Read.o: src/Read.cpp include/Read.h
	g++ $(CFLAGS) -o bin/Read.o src/Read.cpp

bin/User.o: src/User.cpp include/User.h
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

bin/Write.o: src/Write.cpp include/Write.h
	g++ $(CFLAGS) -o bin/Write.o src/Write.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
