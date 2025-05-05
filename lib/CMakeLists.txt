add_library( challenges  
   src/RowCol.cpp
)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  CHECK_COMPILERER_FLAG("/std:c++20" _cpp_20_flag_supported)
  if (_cpp_20_flag_supported)
    target_compile_options(challenges "/std:c++20")
  endif()
  target_compile_options( challenges "/WX" "/MP")
else()
   target_compile_options(challenges PUBLIC -std=c++20)

endif()

target_include_directories( challenges 
    PUBLIC include
)

add_subdirectory( parser )    
