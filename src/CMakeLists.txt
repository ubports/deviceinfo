add_library(${TARGET} SHARED
    iniparser.cpp
    config.cpp
    device.cpp
    deviceinfo.cpp
    logger.cpp
)

if(ANDROIDPROPS_FOUND)
    add_definitions(-DHAVE_PROPS)
endif()

target_link_libraries(${TARGET}
    ${ANDROIDPROPS_LDFLAGS}
)

set_target_properties(
        ${TARGET}

        PROPERTIES
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
        SOVERSION ${VERSION_MAJOR}
)

install(TARGETS ${TARGET} DESTINATION ${CMAKE_INSTALL_LIBDIR})
