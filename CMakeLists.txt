cmake_minimum_required(VERSION 3.22)

project(lessonPlanner VERSION 1.0.0 LANGUAGES C CXX)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generowanie dokumentacji Doxygen"
        VERBATIM
    )
endif()

#C++ 20 settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#CMake flags (in order to do not need any other libraries)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

set(EXECUTABLE_NAME lessonPlanner)
set(LIBRARY_NAME Library)

add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(inc)
add_subdirectory(app)
