cmake_minimum_required(VERSION 3.5)
project(slider)

set(CMAKE_CXX_STANDARD 11)
set(EXE slider)

# set src/header
set(SOURCE_FILES
        src/main.cpp
        src/slider/Slider.cc
        src/slider/Board.cc
        src/commons/util.cc
        include/commons/ai/agent/Minimax.h
        include/commons/ai/Strategy.h
        include/slider/Board.h
        include/slider/Move.h
        include/slider/Slider.h
        include/commons/util.h
        include/slider/slider_utils.h
        include/slider/evaluation_functions/basic_eval.h
        include/slider/Referee.cc
        include/slider/Referee.h
        src/slider/io/SliderIO.cc
        include/slider/io/SliderIO.h src/slider/evaluation_functions/basic_eval.h.cc src/slider/gui/SliderGUI.cc include/slider/gui/SliderGUI.h src/slider/Move.cc)

include_directories(include
        include/commons
        include/commons/ai
        include/slider
        include/slider/io
        include/slider/evaluation_functions)

add_executable(${EXE} ${SOURCE_FILES})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 1.6 REQUIRED system window graphics network audio)
target_link_libraries(${EXE} ${SFML_LIBRARIES})
