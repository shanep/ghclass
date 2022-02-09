add_executable(ghc main.cpp)
target_link_libraries(ghc LINK_PUBLIC ghclass)
target_include_directories(ghc PRIVATE ${CMAKE_BINARY_DIR})

## Set compiler flags
target_compile_options(ghc PRIVATE
  "$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>"
  "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Werror;-pedantic;>"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_sanitizers(ghc)
endif()

install(TARGETS ghc DESTINATION bin)
