include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

add_definitions(-DENABLE_LEGACY=1)
add_definitions(-DTEST_DEFAULT_DIR="${TEST_DATA_DIR}")
add_definitions(-DTEST_DEVICE_DIR="${TEST_DATA_DIR}/devices")



macro(declare_test test testname)
    message(${testname})
    add_executable(${testname} ${testname}.cpp)
    target_link_libraries(${testname}
        ${TARGET}

        ${GTEST_LIBRARIES}
        ${GTEST_BOTH_LIBRARIES}
    )

    if(TEST_XML_OUTPUT)
        set(TEST_ARGS -p -xunitxml -p -o -p ${testname}.xml)
    else()
        set(TEST_ARGS "")
    endif()

    add_test(${test} ${testname})
    #set_tests_properties(${testname} PROPERTIES
    #                     TIMEOUT ${CTEST_TESTING_TIMEOUT}
    #                     ENVIRONMENT "QT_QPA_PLATFORM=minimal")

    set(TESTS ${TESTS} ${testname})
endmacro(declare_test testname)

declare_test(TestConfig tst_config)

find_package(CoverageReport)
enable_coverage_report(
  TARGETS
    deviceinfo
  FILTER
    ${CMAKE_SOURCE_DIR}/tests/*
    ${CMAKE_BINARY_DIR}/*
  TESTS
    ${TESTS}
)
