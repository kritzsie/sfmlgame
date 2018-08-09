cmake_minimum_required(VERSION 3.8.2)

project(sfmlgame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PhysFS REQUIRED)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics network system window)

set(
  FMT_SOURCES
  fmt/fmt/format.cc
  fmt/fmt/ostream.cc
  fmt/fmt/posix.cc
  fmt/fmt/printf.cc
)

set(
  SNES_SPC_SOURCES
  snes_spc-0.9.0/snes_spc/SPC_DSP.cpp
  snes_spc-0.9.0/snes_spc/dsp.cpp
  snes_spc-0.9.0/snes_spc/SNES_SPC.cpp
  snes_spc-0.9.0/snes_spc/SPC_Filter.cpp
  snes_spc-0.9.0/snes_spc/SNES_SPC_state.cpp
  snes_spc-0.9.0/snes_spc/SNES_SPC_misc.cpp
  snes_spc-0.9.0/snes_spc/spc.cpp
)

include_directories(json/include)
include_directories(snes_spc-0.9.0)

add_executable(
  sfmlgame
  ${SNES_SPC_SOURCES}
  src/util/file.cpp
  src/assetmanager.cpp
  src/engine.cpp
  src/entity.cpp
  src/music.cpp
  src/renderer.cpp
  src/sound.cpp
  src/world.cpp
  src/main.cpp
)

target_link_libraries(
  sfmlgame
  physfs
  sfml-audio sfml-graphics sfml-network sfml-system sfml-window
)
