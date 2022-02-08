

add_library(ghclass ghclass.cpp)
add_executable(ghc ghclass.cpp main.cpp)


include(GenerateExportHeader)
generate_export_header(ghclass)

target_include_directories(ghclass PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

## Set compiler flags for the library
target_compile_options(ghclass PRIVATE
  "$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>"
  "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Werror;-pedantic;>"
)

add_sanitizers(ghclass)

install(TARGETS ghclass DESTINATION lib)
