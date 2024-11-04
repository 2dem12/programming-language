cmake_minimum_required(VERSION 3.28)
project(LangugePrigram)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DWORD="${CMAKE_SOURCE_DIR}/lexem/wordsList.txt")
add_definitions(-DCODE="${CMAKE_SOURCE_DIR}/code.txt")

add_executable(LangugePrigram main.cpp)
