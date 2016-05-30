file(GLOB fileInputReaderTest
    "./*"
    "../src/*"
)
add_executable(fileInputReaderTest ${fileInputReaderTest})
TARGET_LINK_LIBRARIES(fileInputReaderTest cppunit)
set_target_properties(fileInputReaderTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(FileInputReader ${PROJECT_SOURCE_DIR}/dist/tests/fileInputReaderTest)
