FILE(GLOB systemd_file "${CMAKE_CURRENT_SOURCE_DIR}/deviceinfo-refresh.service")
FILE(GLOB upstart_file "${CMAKE_CURRENT_SOURCE_DIR}/deviceinfo-refresh.conf")

pkg_check_modules(SYSTEMD systemd)
if (${SYSTEMD_FOUND})
  pkg_get_variable(SYSTEMD_SYSTEM_DIR systemd systemdsystemunitdir)
  message (STATUS "${SYSTEMD_SYSTEM_DIR} is the systemd system unit file install dir")
  message(STATUS "${systemd_file} is your file")
  # install it
  install (FILES "${systemd_file}"
           DESTINATION "${SYSTEMD_SYSTEM_DIR}")
endif()

if (ENABLE_UPSTART)
    message("Installing upstart files")
    install(FILES ${upstart_file}
        DESTINATION /etc/init
    )
endif()
