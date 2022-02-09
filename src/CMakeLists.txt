add_library(ghclass ghclass.cpp)


include(GenerateExportHeader)
generate_export_header(ghclass)

## Set the include directories for both the headers
## and the generated export headers for linking
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
