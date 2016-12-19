cmake_minimum_required (VERSION 2.6)
project (uadet2 CXX)

file(GLOB CLIENT_SRC
    "src/client.cpp"
    "src/common/src/*"
    "src/FileInputReader/src/*"
    "src/Network/src/*"
    "src/Client/src/*"
)

file(GLOB SERVER_SRC    
    "src/Server/src/*"
    "src/server.cpp"
    "src/common/src/*"
    "src/NaiveBayessClassifier/src/*"
    "src/NgramBuilder/src/*"
    "src/FileInputReader/src/*"
    "src/UATokenizer/src/*"
    "src/Network/src/*"
)

add_executable(uadet2d ${SERVER_SRC})
add_executable(uadet2cli ${CLIENT_SRC})

SET(CMAKE_CXX_FLAGS_DEBUG "-pthread -lpthread") 
SET(CMAKE_CXX_FLAGS_RELEASE "-pthread -lpthread") 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/dist)
set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(CMAKE_BUILD_TYPE Debug)

# 
# TESTS
#
enable_testing()

# FileInputReader
add_subdirectory(src/FileInputReader/test)

# NaiveBayessClassifier
add_subdirectory(src/NaiveBayessClassifier/test)

# NgramBuilder
add_subdirectory(src/NgramBuilder/test)

# UATokenizer
add_subdirectory(src/UATokenizer/test)

# Common Tests
add_subdirectory(src/common/test)

# Network Tests
add_subdirectory(src/Network/test)

# Client Tests
add_subdirectory(src/Client/test)

# Server class Tests
add_subdirectory(src/Server/test)
