add_executable( challenge03 
    src/main.cpp
    src/commandline.cpp
)

target_include_directories( challenge03
    PUBLIC
    include)

target_link_libraries( challenge03
    PRIVATE clipp)
