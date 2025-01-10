add_executable( challenge01v2 
    main.cpp
    commandline.cpp
    )



#Installation commands
include(GNUInstallDirs)
install( TARGETS challenge01
         CONFIGURATIONS Release
         DESTINATION ${CMAKE_INSTALL_BINDIR}/Challenge01v2
         )

install( TARGETS challenge01
    CONFIGURATIONS Debug
    DESTINATION ${CMAKE_INSTALL_BINDIR}/Debug/Challenge01v2
)


