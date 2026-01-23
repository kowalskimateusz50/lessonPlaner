add_subdirectory(OpenXLSX)
target_link_libraries(${LIBRARY_NAME} PUBLIC OpenXLSX)
add_subdirectory(yaml-cpp)
target_link_libraries(${LIBRARY_NAME} PUBLIC yaml-cpp)

