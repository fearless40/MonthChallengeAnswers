add_executable(challenge02
    src/main.cpp
    src/commandline.cpp
    src/createmode.cpp
    src/verifymode.cpp
    src/fileloader.cpp
    src/filewriter.cpp
    src/game.cpp
)

target_include_directories( challenge02
 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} include/
)

target_link_libraries(  challenge02
    PRIVATE challenges
    PRIVATE clipp
    PRIVATE parser
    )
