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

#set(SOURCE
#    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
#  )
add_executable(uadet2 ${ALL_SRC})
set_property(TARGET uadet2 PROPERTY CXX_STANDARD 11)
set_property(TARGET uadet2 PROPERTY CXX_STANDARD_REQUIRED ON)
# CMAKE_REQUIRED_FLAGS=-std=c++11
#CHECK_CXX_COMPILER_FLAG(std c++11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/dist)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/)

set(CMAKE_CXX_FLAGS "-std=c++11")

#add_subdirectory("${PROJECT_SOURCE_DIR}/common")

#include_directories(${PROJECT_SOURCE_DIR}/NaiveBayessClassifier)
#add_subdirectory(NaiveBayessClassifier)
#target_link_libraries (uadet2 NaiveBayessClassifier)

#include_directories(${PROJECT_SOURCE_DIR}/NgramBuilder)
#add_subdirectory(NgramBuilder)
#target_link_libraries (uadet2 NgramBuilder)

#include_directories(${PROJECT_SOURCE_DIR}/FileInputReader)
#add_subdirectory(FileInputReader)
#target_link_libraries (uadet2 FileInputReader)


#include_directories(${PROJECT_SOURCE_DIR}/UATokenizer)
#add_subdirectory(UATokenizer)
#target_link_libraries (uadet2 UATokenizer)

