include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

add_definitions(-DTEST_DATA_DIR="${TEST_DATA_DIR}")

add_executable(tst_iniparser tst_iniparser.cpp)
target_link_libraries(tst_iniparser
    ${TARGET}

    ${GTEST_LIBRARIES}
    ${GTEST_BOTH_LIBRARIES}
)
add_test(IniParser tst_iniparser)
