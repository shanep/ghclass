cmake_minimum_required(VERSION 3.13.0)
project(ghclass VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(git2 REQUIRED libgit2)

include(CTest)
enable_testing()

add_executable(ghclass main.cpp)
target_include_directories(ghclass PRIVATE ${git2_INCLUDE_DIRS})
target_link_directories(ghclass PRIVATE ${git2_LIBRARY_DIRS})
target_link_libraries(ghclass PRIVATE ${git2_LIBRARIES})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
