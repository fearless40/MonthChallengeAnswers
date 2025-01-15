include(FetchContent)


FetchContent_Declare( clipp 
    GIT_REPOSITORY https://github.com/GerHobbelt/clipp
    GIT_TAG master
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/clipp"
    GIT_PROGRESS True
    EXCLUDE_FROM_ALL "examples"
    FETCHCONTENT_UPDATES_DISCONNECTED true
)

FetchContent_MakeAvailable( clipp )
