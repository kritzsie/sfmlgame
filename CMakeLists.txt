cmake_minimum_required(VERSION 3.8.2)

project(sfmlgame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2 REQUIRED graphics system window)
include_directories(${SFML_INCLUDE_DIR})

set(
  FMT_SOURCES
  src/fmt/format.cc
  src/fmt/ostream.cc
  src/fmt/posix.cc
  src/fmt/printf.cc
)

add_executable(
  sfmlgame
  ${FMT_SOURCES}
  src/engine.cpp
  src/geometry.cpp
  src/main.cpp
)

target_link_libraries(
  sfmlgame
  ${SFML_LIBRARIES}
)
