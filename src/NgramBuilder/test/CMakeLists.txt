# NgramBuilder
file(GLOB f3
    "./*"
    "../src/*"
    "../../UATokenizer/src/*"
)
add_executable(f3 ${f3})
TARGET_LINK_LIBRARIES(f3 cppunit)
set_target_properties(f3 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(NgramBuilder ${PROJECT_SOURCE_DIR}/dist/tests/f3)
