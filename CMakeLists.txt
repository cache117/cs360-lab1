cmake_minimum_required(VERSION 3.3)
project(Lab1)
add_definitions(-DWIN32)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#server
set(SERVER_FILES server.cpp)

add_executable(server ${SERVER_FILES})
target_link_libraries(server wsock32 ws2_32)

#download
set(DOWNLOAD_FILES download.cpp CS360Utils.h)
add_executable(download ${DOWNLOAD_FILES})
target_link_libraries(download wsock32 ws2_32)