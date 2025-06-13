add_executable( challenge03 
    src/main.cpp
    src/commandline.cpp
    src/stupidai.cpp
    src/guessallai.cpp
    src/averageai.cpp
)

target_include_directories( challenge03
    PUBLIC
    include)

target_link_libraries( challenge03
    PRIVATE clipp
    PRIVATE challenges)
