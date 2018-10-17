cmake_minimum_required(VERSION 3.9)
project(smak)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(smak client.cpp client.h main.cpp main.h driver.cpp tcpServerSocket.cpp tcpServerSocket.h tcpUserSocket.cpp tcpUserSocket.h utils.h driver.h)
target_link_libraries(smak Threads::Threads)

