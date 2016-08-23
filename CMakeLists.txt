cmake_minimum_required(VERSION 3.6)

project(sfmlgame)

find_package(SFML 2 REQUIRED graphics system window)
include_directories(${SFML_INCLUDE_DIR})

add_executable(sfmlgame
  src/engine.cpp
  src/geometry.cpp
  src/main.cpp
)

target_link_libraries(sfmlgame ${SFML_LIBRARIES})
