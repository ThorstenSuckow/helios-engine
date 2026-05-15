set(HELIOS_ECS_LOCAL_PATH "${CMAKE_CURRENT_LIST_DIR}/../../helios-ecs" CACHE PATH "Local path to helios-ecs repository")
set(HELIOS_MATH_LOCAL_PATH "${CMAKE_CURRENT_LIST_DIR}/../../helios-math" CACHE PATH "Local path to helios-math repository")

# Do not build dependency test/benchmark targets as part of engine.
set(_HELIOS_ENGINE_BUILD_TESTING "${BUILD_TESTING}")
set(_HELIOS_ENGINE_BUILD_BENCHMARKS "${BUILD_BENCHMARKS}")
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

if(NOT TARGET helios::ecs)
    if(EXISTS "${HELIOS_ECS_LOCAL_PATH}/CMakeLists.txt")
        add_subdirectory("${HELIOS_ECS_LOCAL_PATH}" "${CMAKE_BINARY_DIR}/_deps/helios-ecs-build")
        message(STATUS "Using local helios-ecs: ${HELIOS_ECS_LOCAL_PATH}")
    else()
        FetchContent_Declare(helios_ecs
                GIT_REPOSITORY https://github.com/thorstensuckow/helios-ecs.git
                GIT_TAG main
        )
        FetchContent_MakeAvailable(helios_ecs)
    endif()
endif()

if(NOT TARGET helios::math)
    if(EXISTS "${HELIOS_MATH_LOCAL_PATH}/CMakeLists.txt")
        add_subdirectory("${HELIOS_MATH_LOCAL_PATH}" "${CMAKE_BINARY_DIR}/_deps/helios-math-build")
        message(STATUS "Using local helios-math: ${HELIOS_MATH_LOCAL_PATH}")
    else()
        FetchContent_Declare(helios_math
                GIT_REPOSITORY https://github.com/thorstensuckow/helios-math.git
                GIT_TAG main
        )
        FetchContent_MakeAvailable(helios_math)
    endif()
endif()

if(DEFINED _HELIOS_ENGINE_BUILD_TESTING)
    set(BUILD_TESTING "${_HELIOS_ENGINE_BUILD_TESTING}" CACHE BOOL "" FORCE)
endif()
if(DEFINED _HELIOS_ENGINE_BUILD_BENCHMARKS)
    set(BUILD_BENCHMARKS "${_HELIOS_ENGINE_BUILD_BENCHMARKS}" CACHE BOOL "" FORCE)
endif()

