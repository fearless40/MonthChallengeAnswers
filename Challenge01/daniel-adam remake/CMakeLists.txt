# CMakeList.txt : CMake project for Battleship Test 1Make, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (RemakeDaniel "Battleship Test 1Make.cpp" "Battleship Test 1Make.h" "Helper.h" "UserInput.cpp" "UserInput.h" "FileFuncs.cpp" "FileFuncs.h" Helper.cpp)


# TODO: Add tests and install targets if needed.



#Installation commands
include(GNUInstallDirs)
install( TARGETS RemakeDaniel
         CONFIGURATIONS Release
         DESTINATION ${CMAKE_INSTALL_BINDIR}/Challenge01
         )

install( TARGETS RemakeDaniel
    CONFIGURATIONS Debug
    DESTINATION ${CMAKE_INSTALL_BINDIR}/Debug/Challenge01
)


