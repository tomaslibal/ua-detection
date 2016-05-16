# NgramBuilder
file(GLOB network
    "./*"
    "../src/*"
    "../../common/src/FileLog.*"
    "../../common/src/uadet2.*"
)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
add_executable(network ${network})
TARGET_LINK_LIBRARIES(network cppunit)
set_target_properties(network PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(Network ${PROJECT_SOURCE_DIR}/dist/tests/network)
