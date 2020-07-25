cmake_minimum_required(VERSION 3.0.0)
project(ghclass VERSION 0.1.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_CXX_STANDARD 11)

find_package(git2 REQUIRED)

include(CTest)
enable_testing()

add_executable(ghclass main.cpp)
target_include_directories(ghclass PRIVATE ${GIT2_INCLUDE_PATH})
target_link_libraries(ghclass PRIVATE ${GIT2_LIBRARY})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
