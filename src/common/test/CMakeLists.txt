# ProgramConfig Test
file(GLOB programConfigTest
    "./programConfigTest.h"
    "./programConfigTest.cpp"
    "./programConfigTestRunner.cpp"
    "../src/ProgramConfig.h"
    "../src/ProgramConfig.cpp"
    "../../FileInputReader/src/*"
)
add_executable(programConfigTest ${programConfigTest})
TARGET_LINK_LIBRARIES(programConfigTest cppunit)
set_target_properties(programConfigTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(ProgramConfig ${PROJECT_SOURCE_DIR}/dist/tests/programConfigTest)

file(GLOB fileLogTest
    "./fileLogTest.*"
    "./fileLogTestRunner.cpp"
    "../src/FileLog.*"
)
add_executable(fileLogTest ${fileLogTest})
TARGET_LINK_LIBRARIES(fileLogTest cppunit)
set_target_properties(fileLogTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(FileLog ${PROJECT_SOURCE_DIR}/dist/tests/fileLogTest)

file(GLOB commandTest
    "./commandTest*"
    "../src/Command.*"
)
add_executable(commandTest ${commandTest})
TARGET_LINK_LIBRARIES(commandTest cppunit)
set_target_properties(commandTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(CommandTest ${PROJECT_SOURCE_DIR}/dist/tests/commandTest)

file(GLOB uadet2commonTest
    "./uadet2Test*"
    "../src/uadet2.*"
)
add_executable(uadet2commonTest ${uadet2commonTest})
TARGET_LINK_LIBRARIES(uadet2commonTest cppunit)
set_target_properties(uadet2commonTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist/tests)
add_test(CommandTest ${PROJECT_SOURCE_DIR}/dist/tests/uadet2commonTest)
