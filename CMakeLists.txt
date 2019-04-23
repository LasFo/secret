cmake_minimum_required (VERSION 3.1)
project (present)

include_directories("SFML-2.5.1/include")
link_directories("SFML-2.5.1/lib")

add_executable(game main.cpp)
target_link_libraries(game sfml-graphics)
target_link_libraries(game sfml-window)
target_link_libraries(game sfml-system)