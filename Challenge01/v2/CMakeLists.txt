add_executable( challenge01v2 
    main.cpp
    commandline.cpp
    )



#Installation commands
include(GNUInstallDirs)
install( TARGETS challenge01v2
         CONFIGURATIONS Release
         DESTINATION ${CMAKE_INSTALL_BINDIR}/Challenge01
         )

install( TARGETS challenge01v2
    CONFIGURATIONS Debug
    DESTINATION ${CMAKE_INSTALL_BINDIR}/Debug/Challenge01
)


