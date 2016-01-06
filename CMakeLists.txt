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

file(GLOB CLIENT_SRC
    "client.cpp"
)

file(GLOB SERVER_SRC    
    "server/src/*.h"
    "server/src/*.cpp"
    "server.cpp"
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

add_executable(uadet2d ${SERVER_SRC})
add_executable(uadet2cli ${CLIENT_SRC})

SET(CMAKE_CXX_FLAGS_DEBUG "-pthread -lpthread") 
SET(CMAKE_CXX_FLAGS_RELEASE "-pthread -lpthread") 

# More portable threading:
#find_package (Threads REQUIRED)
#if(THREADS_HAVE_PTHREAD_ARG)
#  set_property(TARGET uadet2d PROPERTY COMPILE_OPTIONS "-pthread")
#  set_property(TARGET uadet2d PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
#endif()
#if(CMAKE_THREAD_LIBS_INIT)
#  target_link_libraries(uadet2d "${CMAKE_THREAD_LIBS_INIT}")
#endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/dist)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_BUILD_TYPE Debug)
