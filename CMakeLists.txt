cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
link_libraries("-lboost_system -pthread")

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp src/Write.cpp include/User.h src/User.cpp src/Read.cpp include/Read.h include/Write.h src/MainClient.cpp)
