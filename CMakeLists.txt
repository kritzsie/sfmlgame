cmake_minimum_required(VERSION 2.6)
project(sfmlgame)

add_executable(sfmlgame main.cpp)

install(TARGETS sfmlgame RUNTIME DESTINATION bin)
