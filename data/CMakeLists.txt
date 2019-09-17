set(CONFIG_FILES
    # Default
    defaults.conf

    # Device spesific
    pinephone.conf
)

install(FILES ${CONFIG_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/device-info)
