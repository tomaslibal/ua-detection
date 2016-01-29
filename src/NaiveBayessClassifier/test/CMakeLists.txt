# NaiveBayessClassifier
file(GLOB f2
    "./*"
    "../src/*"
    "../../NgramBuilder/src/*"
    "../../UATokenizer/src/*"
)
add_executable(f2 ${f2})
TARGET_LINK_LIBRARIES(f2 cppunit)
set_target_properties(f2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(NaiveBayessClassifier ${PROJECT_SOURCE_DIR}/dist/tests/f2)
