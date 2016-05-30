# NaiveBayessClassifier
file(GLOB nbClassifierTest
    "./*"
    "../src/*"
    "../../NgramBuilder/src/*"
    "../../UATokenizer/src/*"
)
add_executable(nbClassifierTest ${nbClassifierTest})
TARGET_LINK_LIBRARIES(nbClassifierTest cppunit)
set_target_properties(nbClassifierTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(NaiveBayessClassifier ${PROJECT_SOURCE_DIR}/dist/tests/nbClassifierTest)
