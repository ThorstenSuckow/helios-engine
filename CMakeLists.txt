cmake_minimum_required(VERSION 4.0)
project(helios-engine LANGUAGES C CXX)

include(CTest)
include(FetchContent)

include(cmake/ProjectOptions.cmake)
include(cmake/Dependencies.cmake)
include(cmake/ThirdParty.cmake)

add_library(helios STATIC)
add_library(helios::helios ALIAS helios)
set_property(TARGET helios PROPERTY CXX_SCAN_FOR_MODULES ON)
target_compile_features(helios PUBLIC cxx_std_23)

add_subdirectory(src)

target_link_libraries(helios
        PUBLIC
        $<BUILD_INTERFACE:helios::ecs>
        $<BUILD_INTERFACE:helios::math>
        $<INSTALL_INTERFACE:helios::ecs>
        $<INSTALL_INTERFACE:helios::math>
        PRIVATE
        $<BUILD_INTERFACE:thirdparty::freetype>
        $<BUILD_INTERFACE:thirdparty::opengl>
        $<BUILD_INTERFACE:thirdparty::imgui>
)

target_include_directories(helios PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

set(HELIOS_ENGINE_ENABLE_PACKAGE_DEFAULT OFF)
option(HELIOS_ENGINE_ENABLE_PACKAGE "Enable install/export package files" ${HELIOS_ENGINE_ENABLE_PACKAGE_DEFAULT})
if(HELIOS_ENGINE_ENABLE_PACKAGE)
    include(cmake/Install.cmake)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
