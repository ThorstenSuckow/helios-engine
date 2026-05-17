cmake_minimum_required(VERSION 4.0)
project(helios-engine LANGUAGES C CXX)

include(CTest)
include(FetchContent)

include(cmake/ProjectOptions.cmake)
include(cmake/Dependencies.cmake)
include(cmake/ThirdParty.cmake)

add_library(helios_engine STATIC)
add_library(helios::engine ALIAS helios_engine)
set_property(TARGET helios_engine PROPERTY CXX_SCAN_FOR_MODULES ON)
set_target_properties(helios_engine PROPERTIES EXPORT_NAME engine)
target_compile_features(helios_engine PUBLIC cxx_std_23)

add_subdirectory(src)

target_link_libraries(helios_engine
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

target_include_directories(helios_engine PUBLIC
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
