# UATokenizer
file(GLOB f4
    "./*"
    "../src/*"
)
add_executable(f4 ${f4})
TARGET_LINK_LIBRARIES(f4 cppunit)
set_target_properties(f4 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(UATokenizer ${PROJECT_SOURCE_DIR}/dist/tests/f4)
