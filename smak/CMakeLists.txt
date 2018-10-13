cmake_minimum_required(VERSION 3.9)
project(smak)

set(CMAKE_CXX_STANDARD 11)

add_executable(smak Server.cpp Server.h Srv.cpp client.cpp client.h main.cpp Session.cpp Session.h)