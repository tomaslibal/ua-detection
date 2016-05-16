# NgramBuilder
file(GLOB network
    "./*"
    "../src/*"
    "../../common/src/FileLog.*"
)
add_executable(network ${network})
TARGET_LINK_LIBRARIES(network cppunit)
set_target_properties(network PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(Network ${PROJECT_SOURCE_DIR}/dist/tests/network)
