cmake_minimum_required(VERSION 4.0)
project(helios-engine LANGUAGES C CXX)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

include(CTest)
include(FetchContent)

if(CMAKE_IFC_OUTPUT_DIRECTORY)
    file(MAKE_DIRECTORY "${CMAKE_IFC_OUTPUT_DIRECTORY}")
endif()

set(HELIOS_ECS_LOCAL_PATH "${CMAKE_CURRENT_LIST_DIR}/../helios-ecs" CACHE PATH "Local path to helios-ecs repository")
set(HELIOS_MATH_LOCAL_PATH "${CMAKE_CURRENT_LIST_DIR}/../helios-math" CACHE PATH "Local path to helios-math repository")
if(EXISTS "${HELIOS_ECS_LOCAL_PATH}/src/_module.ixx")
    set(HELIOS_ECS_SOURCE_DIR "${HELIOS_ECS_LOCAL_PATH}")
    message(STATUS "Using local helios-ecs: ${HELIOS_ECS_SOURCE_DIR}")
else()
    FetchContent_Declare(helios_ecs
            GIT_REPOSITORY https://github.com/thorstensuckow/helios-ecs.git
            GIT_TAG main
    )
    FetchContent_GetProperties(helios_ecs)
    if(NOT helios_ecs_POPULATED)
        FetchContent_Populate(helios_ecs)
    endif()
    set(HELIOS_ECS_SOURCE_DIR "${helios_ecs_SOURCE_DIR}")
    message(STATUS "Using remote helios-ecs: ${HELIOS_ECS_SOURCE_DIR}")
endif()

if(EXISTS "${HELIOS_MATH_LOCAL_PATH}/src/_module.ixx")
    set(HELIOS_MATH_SOURCE_DIR "${HELIOS_MATH_LOCAL_PATH}")
    message(STATUS "Using local helios-math: ${HELIOS_MATH_SOURCE_DIR}")
else()
    FetchContent_Declare(helios_math
            GIT_REPOSITORY https://github.com/thorstensuckow/helios-math.git
            GIT_TAG main
    )
    FetchContent_GetProperties(helios_math)
    if(NOT helios_math_POPULATED)
        FetchContent_Populate(helios_math)
    endif()
    set(HELIOS_MATH_SOURCE_DIR "${helios_math_SOURCE_DIR}")
    message(STATUS "Using remote helios-math: ${HELIOS_MATH_SOURCE_DIR}")
endif()

# Third-party dependencies required by engine modules.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_FREETYPE_DEBUGGER ON CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_Declare(freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG VER-2-14-1
)
FetchContent_Declare(glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v2.0.8
        SOURCE_SUBDIR cmake
)
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_MakeAvailable(freetype glfw glad imgui)

if(NOT TARGET thirdparty::glad)
    glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE LOADER API gl:core=4.6)
    add_library(thirdparty::glad ALIAS glad_gl_core_46)
endif()
if(NOT TARGET thirdparty::freetype)
    add_library(thirdparty::freetype ALIAS freetype)
endif()
if(NOT TARGET thirdparty::glfw)
    add_library(thirdparty::glfw ALIAS glfw)
endif()
if(NOT TARGET thirdparty_opengl)
    add_library(thirdparty_opengl INTERFACE)
    target_link_libraries(thirdparty_opengl INTERFACE thirdparty::glad thirdparty::glfw)
    add_library(thirdparty::opengl ALIAS thirdparty_opengl)
endif()
if(NOT TARGET imgui)
    add_library(imgui STATIC
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(imgui PUBLIC thirdparty::glfw thirdparty::glad)
endif()
if(NOT TARGET thirdparty::imgui)
    add_library(thirdparty::imgui ALIAS imgui)
endif()

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE MODULE_INTERFACE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ixx")
file(GLOB_RECURSE HELIOS_ECS_CPP_FILES CONFIGURE_DEPENDS
        "${HELIOS_ECS_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HELIOS_ECS_MODULE_INTERFACE_FILES CONFIGURE_DEPENDS
        "${HELIOS_ECS_SOURCE_DIR}/src/*.ixx")
file(GLOB_RECURSE HELIOS_MATH_CPP_FILES CONFIGURE_DEPENDS
        "${HELIOS_MATH_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HELIOS_MATH_MODULE_INTERFACE_FILES CONFIGURE_DEPENDS
        "${HELIOS_MATH_SOURCE_DIR}/src/*.ixx")

list(APPEND CPP_FILES ${HELIOS_ECS_CPP_FILES})
list(APPEND MODULE_INTERFACE_FILES ${HELIOS_ECS_MODULE_INTERFACE_FILES})
list(APPEND CPP_FILES ${HELIOS_MATH_CPP_FILES})
list(APPEND MODULE_INTERFACE_FILES ${HELIOS_MATH_MODULE_INTERFACE_FILES})

list(FILTER CPP_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/build/")
list(FILTER MODULE_INTERFACE_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/build/")

add_library(helios STATIC)
set_property(TARGET helios PROPERTY CXX_SCAN_FOR_MODULES ON)
target_compile_features(helios PUBLIC cxx_std_23)
target_sources(helios PRIVATE ${CPP_FILES})

if(MODULE_INTERFACE_FILES)
    target_sources(helios PUBLIC
            FILE_SET modules TYPE CXX_MODULES
            BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
            "${HELIOS_ECS_SOURCE_DIR}/src"
            "${HELIOS_MATH_SOURCE_DIR}/src"
            FILES ${MODULE_INTERFACE_FILES}
    )
endif()

target_link_libraries(helios
        PRIVATE
        $<BUILD_INTERFACE:thirdparty::freetype>
        $<BUILD_INTERFACE:thirdparty::opengl>
        $<BUILD_INTERFACE:thirdparty::imgui>
)

target_include_directories(helios PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${HELIOS_ECS_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${HELIOS_MATH_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

set(HELIOS_ENGINE_ENABLE_PACKAGE_DEFAULT OFF)
if(PROJECT_IS_TOP_LEVEL)
    set(HELIOS_ENGINE_ENABLE_PACKAGE_DEFAULT ON)
endif()
option(HELIOS_ENGINE_ENABLE_PACKAGE "Enable install/export package files" ${HELIOS_ENGINE_ENABLE_PACKAGE_DEFAULT})
if(HELIOS_ENGINE_ENABLE_PACKAGE)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS helios
            EXPORT helios-engineTargets
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            FILE_SET modules DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(FILES
            "${HELIOS_ECS_SOURCE_DIR}/src/helios-ecs-config.h"
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
endif()

if(BUILD_TESTING)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.17.0
    )
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 1.0.1
    )
    set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest glm)

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.test.cpp")

    add_executable(helios-engine-tests ${TEST_SOURCES})
    target_link_libraries(helios-engine-tests PRIVATE helios GTest::gtest_main glm::glm)

    include(GoogleTest)
    gtest_discover_tests(helios-engine-tests)
endif()
