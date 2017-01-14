# NaiveBayessClassifier
file(GLOB nbClassifierTest
    "./naiveBayesClassifier*"
    "../src/*"
    "../../NgramBuilder/src/*"
    "../../UATokenizer/src/*"
)
add_executable(nbClassifierTest ${nbClassifierTest})
TARGET_LINK_LIBRARIES(nbClassifierTest cppunit)
set_target_properties(nbClassifierTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(NaiveBayessClassifier ${PROJECT_SOURCE_DIR}/dist/tests/nbClassifierTest)

# NaiveBayessClassifier
file(GLOB nbcCacheTest
    "./NBCCache*"
    "../src/*"
    "../../NgramBuilder/src/*"
    "../../UATokenizer/src/*"
)
add_executable(nbcCacheTest ${nbcCacheTest})
TARGET_LINK_LIBRARIES(nbcCacheTest cppunit)
set_target_properties(nbcCacheTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(nbcCacheTest ${PROJECT_SOURCE_DIR}/dist/tests/nbcCacheTest)
