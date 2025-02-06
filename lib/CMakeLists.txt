add_library( challenges  
   src/RowCol.cpp

)

target_include_directories( challenges 
    PUBLIC include
)

add_subdirectory( parser )    