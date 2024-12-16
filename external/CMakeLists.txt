include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ftxui"
  GIT_PROGRESS True
)



FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_MakeAvailable(ftxui)
endif()


FetchContent_Declare( clipp 
    GIT_REPOSITORY https://github.com/muellan/clipp
    GIT_TAG v1.2.3
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/clipp"
    GIT_PROGRESS True
)

FetchContent_MakeAvailable( clipp )
