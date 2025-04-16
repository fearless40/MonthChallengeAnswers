cmake_minimum_required(VERSION 3.4...3.8)


### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
  message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

project(ChallengeAnswers VERSION 1.0
        DESCRIPTION "Simple programs to solve the monthly challenges"
        LANGUAGES CXX)

set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD  20)
set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD_REQUIRED On)
set_property(GLOBAL PROPERTY CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  CHECK_COMPILERER_FLAG("/std:c++20" _cpp_20_flag_supported)
  if (_cpp_20_flag_supported)
    add_compile_options("/std:c++20")
  endif()
  add_compile_options("/WX" "/MP")
else()
  add_compile_options("-std=c++20")

endif()

add_subdirectory(external)
add_subdirectory(Challenge01)
add_subdirectory(Challenge02)
add_subdirectory(Challenge03)
add_subdirectory(lib)



