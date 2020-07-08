cmake_minimum_required(VERSION 3.16)
project(CocCoc)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(CocCoc
        include/BufferedDataReader.h
        include/CallBack.h
        include/TcpClient.h
        include/EventManager.h
        include/FileDescriptor.h
        include/Math.h
        include/TcpServer.h
        include/Socket.h
        include/StringBuilder.h
        include/Strings.h
        include/ThreadPool.h
        include/Utils.h
        include/Message.h
        include/Event.h
        include/Epoll.h
        common/TcpClient.cpp
        common/EventManager.cpp
        common/FileDescriptor.cpp
        common/Math.cpp
        common/TcpServer.cpp
        common/Socket.cpp
        common/ThreadPool.cpp
        mainServer.cpp common/DataFactory.cpp include/DataFactory.h)