include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

add_definitions(-DTEST_DEFAULT_DIR="${TEST_DATA_DIR}")
add_definitions(-DTEST_DEVICE_DIR="${TEST_DATA_DIR}/devices")

add_executable(tst_config tst_config.cpp)
target_link_libraries(tst_config
    ${TARGET}

    ${GTEST_LIBRARIES}
    ${GTEST_BOTH_LIBRARIES}
)
add_test(Config tst_config)
