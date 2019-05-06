cmake_minimum_required (VERSION 3.1)
project (present)

include_directories("SFML-2.5.1/include")
link_directories("SFML-2.5.1/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

add_library(game-model
            model.cpp)

add_executable(game main.cpp)
target_link_libraries(game sfml-graphics)
target_link_libraries(game sfml-window)
target_link_libraries(game sfml-system)
target_link_libraries(game sfml-audio)
target_link_libraries(game game-model)

if(MSVC)
  target_compile_options(game PRIVATE /W4 /WX)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

