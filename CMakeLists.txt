cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COLOR_MAKEFILE ON)

project(Main)
add_executable(main SMS.cpp )
install(TARGETS main RUNTIME DESTINATION bin)
