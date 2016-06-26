file(GLOB clientTest
    "./*"
    "../src/*"
    "../../Network/src/*"
    "../../FileInputReader/src/*"
    "../../common/src/*"    
)
add_executable(clientTest ${clientTest})
TARGET_LINK_LIBRARIES(clientTest cppunit)
set_target_properties(clientTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(FileInputReader ${PROJECT_SOURCE_DIR}/dist/tests/clientTest)
