cmake_minimum_required (VERSION 2.6)
project (uadet2 CXX)

file(GLOB ALL_SRC
    "src/main.cpp"
    "src/common/src/*.h"
    "src/common/src/*.cpp"
    "src/NaiveBayessClassifier/src/*.h"
    "src/NaiveBayessClassifier/src/*.cpp"
    "src/NgramBuilder/src/*.h"
    "src/NgramBuilder/src/*.cpp"
    "src/FileInputReader/src/*.h"
    "src/FileInputReader/src/*.cpp"
    "src/UATokenizer/src/*.h"
    "src/UATokenizer/src/*.cpp"
)

file(GLOB CLIENT_SRC
    "src/client.cpp"
)

file(GLOB SERVER_SRC    
    "src/server/src/*.h"
    "src/server/src/*.cpp"
    "src/server.cpp"
    "src/common/src/*.h"
    "src/common/src/*.cpp"
    "src/NaiveBayessClassifier/src/*.h"
    "src/NaiveBayessClassifier/src/*.cpp"
    "src/NgramBuilder/src/*.h"
    "src/NgramBuilder/src/*.cpp"
    "src/FileInputReader/src/*.h"
    "src/FileInputReader/src/*.cpp"
    "src/UATokenizer/src/*.h"
    "src/UATokenizer/src/*.cpp"
)

add_executable(uadet2 ${ALL_SRC})
set_property(TARGET uadet2 PROPERTY CXX_STANDARD 11)
set_property(TARGET uadet2 PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(uadet2d ${SERVER_SRC})
add_executable(uadet2cli ${CLIENT_SRC})

SET(CMAKE_CXX_FLAGS_DEBUG "-pthread -lpthread") 
SET(CMAKE_CXX_FLAGS_RELEASE "-pthread -lpthread") 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/dist)
set(CMAKE_CXX_FLAGS "-std=c++11")
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
file(GLOB f3
    "src/NgramBuilder/test/*"    
    "src/NgramBuilder/src/*"    
    "src/UATokenizer/src/*"
)
add_executable(f3 ${f3})
TARGET_LINK_LIBRARIES(f3 cppunit)
set_target_properties(f3 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(NgramBuilder dist/tests/f3)

# UATokenizer
file(GLOB f4
    "src/UATokenizer/test/*"
    "src/UATokenizer/src/*"
)
add_executable(f4 ${f4})
TARGET_LINK_LIBRARIES(f4 cppunit)
set_target_properties(f4 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(UATokenizer dist/tests/f4)

# Common Tests
add_subdirectory(src/common/test)
