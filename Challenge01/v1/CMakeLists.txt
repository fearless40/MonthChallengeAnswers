add_executable( challenge01v1 
    main.cpp
    )



#Installation commands
include(GNUInstallDirs)
install( TARGETS challenge01v1
         CONFIGURATIONS Release
         DESTINATION ${CMAKE_INSTALL_BINDIR}/Challenge01
         )

install( TARGETS challenge01v1
    CONFIGURATIONS Debug
    DESTINATION ${CMAKE_INSTALL_BINDIR}/Debug/Challenge01
)


