# MonthChallengeAnswers
The month challenge answers 

## Challenge 01
Steps to build and run 
1. Clone the repository into your selected directory

``` bash
    git clone https://github.com/fearless40/MonthChallengeAnswers.git
```

2. Enter the directory

``` sh
    cd MonthChallengeAnsweres
```

3. Enter into the build directory

``` sh
    cd build
```

4. Run the following cmake command (ensure you have cmake installed for windows or linux)

For Debug:
``` sh
    cmake .. -D CMAKE_BUILD_TYPE=Debug
```

For Release:
``` sh
    cmake .. -D CMAKE_BUILD_TYPE=Release
```

5. Now build the project depends if you want a release or debug build


For Debug:
``` sh
    cmake --build . -D CMAKE_BUILD_TYPE=Debug
```

For Release:
``` sh
    cmake --build . -D CMAKE_BUILD_TYPE=Release
```

6. If you want to install the binary then run the following command. 

For Debug:
``` sh
    cmake install . --prefix "Directory you want to install" -- config Debug
```

For Release:
``` sh
    cmake install . --prefix "Directory you want to install" -- config Release 
```

7. The file should now be located in the folder you specified