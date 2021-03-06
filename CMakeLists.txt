cmake_minimum_required(VERSION 3.8.2)

project(sfmlgame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

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
  src/entities/baseentity.cpp
  src/entities/camera.cpp
  src/entities/entity.cpp
  src/entities/goalcard.cpp
  src/entities/goomba.cpp
  src/entities/player.cpp
  src/entities/renderentity.cpp
  src/states/intro.cpp
  src/states/basestate.cpp
  src/states/basegame.cpp
  src/states/gameplay.cpp
  src/states/overworld.cpp
  src/states/pausemenu.cpp
  src/util/file.cpp
  src/assetmanager.cpp
  src/engine.cpp
  src/input.cpp
  src/music.cpp
  src/renderer.cpp
  src/sound.cpp
  src/tiletypes.cpp
  src/world.cpp
  src/main.cpp
)

target_link_libraries(
  sfmlgame m physfs
  sfml-audio sfml-graphics sfml-network sfml-system sfml-window
)
