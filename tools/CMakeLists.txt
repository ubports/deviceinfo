add_executable(device-info
    device-info.cpp
)

target_link_libraries(device-info deviceinfo)

install(TARGETS device-info DESTINATION ${CMAKE_INSTALL_BINDIR})
