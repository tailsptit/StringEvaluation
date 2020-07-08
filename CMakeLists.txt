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
        include/DataFactory.h
        include/Expression.h
        src/server/TcpClient.cpp
        src/common/EventManager.cpp
        src/common/FileDescriptor.cpp
        src/math/Math.cpp
        src/server/TcpServer.cpp
        src/common/Socket.cpp
        src/common/ThreadPool.cpp
        src/factory/DataFactory.cpp
        src/math/Expression.cpp
        mainServer.cpp
        )