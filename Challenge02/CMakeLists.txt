add_executable(challenge02 
    src/main.cpp
)

target_include_directories( challenge02 
 PRIVATE include
)

target_link_libraries(  challenge02
    PRIVATE challenges
    INTERFACE clipp
    )