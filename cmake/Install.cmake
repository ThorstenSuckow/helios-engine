include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS helios_engine
        EXPORT helios-engineTargets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        FILE_SET modules DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/helios-engine-config.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

set(HELIOS_ENGINE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/helios-engine")

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/helios-engineConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/helios-engineConfig.cmake"
        INSTALL_DESTINATION "${HELIOS_ENGINE_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/helios-engineConfigVersion.cmake"
        VERSION "0.1.0"
        COMPATIBILITY SameMajorVersion
)

install(EXPORT helios-engineTargets
        FILE helios-engineTargets.cmake
        NAMESPACE helios::
        DESTINATION "${HELIOS_ENGINE_INSTALL_CMAKEDIR}"
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/helios-engineConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/helios-engineConfigVersion.cmake"
        DESTINATION "${HELIOS_ENGINE_INSTALL_CMAKEDIR}"
)

