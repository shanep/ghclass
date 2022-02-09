cmake_minimum_required(VERSION 3.15)

# Don't allow in-source building
if (CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

project(ghclass LANGUAGES CXX VERSION 0.1)

## Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

configure_file(config.h.in config.h)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

## Add address sanatizers for debug builds
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(SANITIZE_ADDRESS TRUE)
  find_package(Sanitizers)
endif()

## Add the library to the build
add_subdirectory(src)
## Add the executable to the build
add_subdirectory(app)
## Add the tests to the build
enable_testing()
add_subdirectory(tests)

## Packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${ghclass_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ghclass_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "ghc")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_SOURCE_IGNORE_FILES
    /.vscode
    /.vagrant
    /.git
    /dist
    /.*build.*
    /\\\\.DS_Store
    solution\.cpp
)
include(CPack)

