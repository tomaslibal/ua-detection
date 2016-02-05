# ProgramConfig Test
file(GLOB f5
    "./programConfigTest.h"
    "./programConfigTest.cpp"
    "./programConfigTestRunner.cpp"
    "../src/ProgramConfig.h"
    "../src/ProgramConfig.cpp"
    "../../FileInputReader/src/*"
)
add_executable(f5 ${f5})
TARGET_LINK_LIBRARIES(f5 cppunit)
set_target_properties(f5 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(ProgramConfig ${PROJECT_SOURCE_DIR}/dist/tests/f5)
