if(ANDROIDPROPS_FOUND)
    add_definitions(-DHAVE_PROPS)

    set(HALIUM_SRC
        platform/halium.cpp
    )
endif()

add_library(${TARGET} SHARED
    config.cpp
    device.cpp
    deviceinfo.cpp
    logger.cpp
    utils.cpp

    platform/platform.cpp
    platform/linux.cpp
    ${HALIUM_SRC}
)

add_definitions(-DENABLE_LEGACY=1)
add_definitions(-DCONFIG_PATH="${CONFIG_PATH}")
add_definitions(-DDEVICES_PATH="${CONFIG_PATH}/devices")

target_link_libraries(${TARGET}
    ${ANDROIDPROPS_LDFLAGS}
    ${YAMLCPP_LDFLAGS}
)

set_target_properties(
        ${TARGET}

        PROPERTIES
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
        SOVERSION ${VERSION_MAJOR}
)

install(TARGETS ${TARGET} DESTINATION ${CMAKE_INSTALL_LIBDIR})
