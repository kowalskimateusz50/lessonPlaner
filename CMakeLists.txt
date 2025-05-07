cmake_minimum_required(VERSION 3.22)

project(lessonPlanner VERSION 1.0.0 LANGUAGES C CXX)

#CMake flags (in order to do not need any other libraries)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

set(EXECUTABLE_NAME lessonPlanner)
set(LIBRARY_NAME Library)

add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(inc)
add_subdirectory(app)
