add_executable( challenge01v1 
    main.cpp
    )

target_link_libraries( challenge01v1 clipp )
target_include_directories( challenge01v1
	PRIVATE ${CMAKE_SOURCE_DIR}/external/clipp/include
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


