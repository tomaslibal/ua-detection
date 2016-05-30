# UATokenizer
file(GLOB uaTokenizerTest
    "./*"
    "../src/*"
)
add_executable(uaTokenizerTest ${uaTokenizerTest})
TARGET_LINK_LIBRARIES(uaTokenizerTest cppunit)
set_target_properties(uaTokenizerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(UATokenizer ${PROJECT_SOURCE_DIR}/dist/tests/uaTokenizerTest)
