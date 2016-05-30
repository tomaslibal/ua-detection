# NgramBuilder
file(GLOB ngram
    "./ngramBuilder*"
    "../src/NgramBuilder*"
)
add_executable(ngram ${ngram})
TARGET_LINK_LIBRARIES(ngram cppunit)
set_target_properties(ngram PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(NgramBuilder ${PROJECT_SOURCE_DIR}/dist/tests/ngram)

file(GLOB uaNgram
    "./uaNgramBuilder*"
    "../src/*"
    "../../UATokenizer/src/*"
)
add_executable(uaNgram ${uaNgram})
TARGET_LINK_LIBRARIES(uaNgram cppunit)
set_target_properties(uaNgram PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(UaNgramBuilder ${PROJECT_SOURCE_DIR}/dist/tests/uaNgram)
