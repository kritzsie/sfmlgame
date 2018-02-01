cmake_minimum_required(VERSION 3.8.2)

project(sfmlgame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2 REQUIRED audio graphics system window)
include_directories(${SFML_INCLUDE_DIR})

include_directories(fmt)
include_directories(physfs-cpp/include)

set(
  FMT_SOURCES
  fmt/fmt/format.cc
  fmt/fmt/ostream.cc
  fmt/fmt/posix.cc
  fmt/fmt/printf.cc
)

add_executable(
  sfmlgame
  ${FMT_SOURCES}
  physfs-cpp/src/physfs.cpp
  src/engine.cpp
  src/entity.cpp
  src/music.cpp
  src/sound.cpp
  src/world.cpp
  src/main.cpp
)

target_link_libraries(
  sfmlgame
  ${SFML_LIBRARIES}
  physfs
)
