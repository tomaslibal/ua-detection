# Server class
file(GLOB serverTest
    "./*"
    "../src/*"
    "../../Network/src/*"
    "../../FileInputReader/src/*"
    "../../common/src/*"  
    "../../UATokenizer/src/*"
    "../../NgramBuilder/src/*"
    "../../NaiveBayessClassifier/src/*"    
)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
add_executable(serverTest ${serverTest})
TARGET_LINK_LIBRARIES(serverTest cppunit)
set_target_properties(serverTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(Server ${PROJECT_SOURCE_DIR}/dist/tests/server)
