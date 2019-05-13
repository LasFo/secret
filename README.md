Steps to install:

1.
    Download the SFML-2.5.1 binaries for your system from https://www.sfml-dev.org/download/sfml/2.5.1/
2. 
    Extract the downloaded binaries in a directory names "SFML-2.5.1" in this directory
3.
    Install cmake https://cmake.org/ (not sure whether you need to install make separately)
    cmake creates make files specifically for you OS which you can then compile with make
4.
    run "cmake CMakeLists.txt" (or use the gui in Windows)
5.
    run "make game" (or use the gui in Windows)
6.
    run the executable "game" ("game.exe" in Windows)

Notes:
    I never tested this on Windows but it should work.
    You MUST start the game from the directory where it was compiled (because I couldn't figure out how to set the Path platform independly correctly).
    You must use a compiler which supports c++17

Example in linux after you downloaded/extracted the SFML binaries and installes cmake:
    1. cmake CMakeLists.txt
    2. make game
    3. ./game
