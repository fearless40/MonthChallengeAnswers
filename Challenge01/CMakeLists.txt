add_executable( challenge01 main.cpp )

# target_link_libraries( challenge01 
#    PUBLIC ftxui::component
#    PUBLIC ftxui::dom
#    PUBLIC ftxui::screen 
#    INTERFACE clipp
#)

#Installation commands
include(GNUInstallDirs)
install( TARGETS challenge01
         CONFIGURATIONS Release
         DESTINATION ${CMAKE_INSTALL_BINDIR}/Challenge01
         )

install( TARGETS challenge01
    CONFIGURATIONS Debug
    DESTINATION ${CMAKE_INSTALL_BINDIR}/Debug/Challenge01
)

