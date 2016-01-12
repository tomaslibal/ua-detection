file(GLOB f1
    "./*"
    "../src/*"
)
add_executable(f1 ${f1})
TARGET_LINK_LIBRARIES(f1 cppunit)
set_target_properties(f1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(FileInputReader ${PROJECT_SOURCE_DIR}/dist/tests/f1)
