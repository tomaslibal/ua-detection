cmake_minimum_required (VERSION 2.6)
project (uadet2 CXX)

file(GLOB ALL_SRC
    "main.cpp"
    "common/src/*.h"
    "common/src/*.cpp"
    "NaiveBayessClassifier/src/*.h"
    "NaiveBayessClassifier/src/*.cpp"
    "NgramBuilder/src/*.h"
    "NgramBuilder/src/*.cpp"
    "FileInputReader/src/*.h"
    "FileInputReader/src/*.cpp"
    "UATokenizer/src/*.h"
    "UATokenizer/src/*.cpp"
)

add_executable(uadet2 ${ALL_SRC})
set_property(TARGET uadet2 PROPERTY CXX_STANDARD 11)
set_property(TARGET uadet2 PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/dist)

set(CMAKE_CXX_FLAGS "-std=c++11")
