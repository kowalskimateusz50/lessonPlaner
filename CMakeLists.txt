cmake_minimum_required(VERSION 3.22)

project(lessonPlanner VERSION 1.0.0 LANGUAGES C CXX)

set(EXECUTABLE_NAME Executable)
set(LIBRARY_NAME Library)
#set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/OpenXLSX/")

add_subdirectory(app)
add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(inc)
