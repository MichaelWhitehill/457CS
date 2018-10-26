add_library(Server netController.cpp netController.h srvState.cpp srvState.h tcpServerSocket.cpp tcpServerSocket.h tcpUserSocket.cpp tcpUserSocket.h driver.cpp driver.h User.cpp User.h)
target_link_libraries(Server)
