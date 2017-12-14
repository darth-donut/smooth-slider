cmake_minimum_required(VERSION 3.5)
project(slider)

set(CMAKE_CXX_STANDARD 11)
set(EXE slider)

# set src/header
set(SOURCE_FILES
        include/commons/ai/agent/Minimax.h
        include/commons/ai/Strategy.h
        include/commons/util.h
        include/slider/Board.h
        include/slider/evaluation_functions/basic_eval.h
        include/slider/gui/SliderGUI.h
        include/slider/io/SliderIO.h
        include/slider/Move.h
        include/slider/Referee.h
        include/slider/Slider.h
        include/slider/slider_utils.h
        src/commons/util.cpp
        src/main.cpp
        src/slider/Board.cpp
        src/slider/evaluation_functions/basic_eval.cpp
        src/slider/gui/SliderGUI.cpp
        src/slider/io/SliderIO.cpp
        src/slider/Referee.cpp
        src/slider/Slider.cpp src/slider/gui/slider_render_window.cpp include/slider/gui/slider_render_window.h src/slider/td_leaf_lambda.cpp include/slider/td_leaf_lambda.h src/model.cpp include/slider/model.h include/commons/resource.h src/learner/Trainer.cpp include/learner/Trainer.h)

include_directories(include/
        include/commons
        include/commons/ai
        include/slider
        include/slider/evaluation_functions
        include/slider/gui
        include/slider/io)

add_executable(${EXE} ${SOURCE_FILES})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 1.6 REQUIRED system window graphics network audio)
target_link_libraries(${EXE} ${SFML_LIBRARIES})

# Multithreading
find_package(Threads REQUIRED)
target_link_libraries(${EXE} ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
