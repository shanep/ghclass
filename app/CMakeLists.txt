add_executable(ghc main.cpp)
target_link_libraries(ghc LINK_PUBLIC ghclass)
target_include_directories(ghc PRIVATE ${CMAKE_BINARY_DIR})

## Set compiler flags
target_compile_options(ghc PRIVATE
  "$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>"
  "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Werror;-pedantic;>"
)

add_sanitizers(ghc)
install(TARGETS ghc DESTINATION bin)
