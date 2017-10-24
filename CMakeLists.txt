cmake_minimum_required(VERSION 3.5)
project(reservation_system)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
add_subdirectory(server)
add_subdirectory(client)