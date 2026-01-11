add_subdirectory(OpenXLSX)
target_link_libraries(${LIBRARY_NAME} PUBLIC OpenXLSX)
message(STATUS "OpenXLSX version: ${OpenXLSX_VERSION}")