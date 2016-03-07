cmake_minimum_required(VERSION 2.8)
enable_language(C CXX)

project(io)

set(PROJECT_MAJOR 0)
set(PROJECT_MINOR 0)
set(PROJECT_PATCH 0)

#if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	include(CTest)
	set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
#	add_custom_target(coverage
#		COMMAND ${PROJECT_SOURCE_DIR}/scripts/gcovr -x -r ${CMAKE_SOURCE_DIR} -o cobertura.xml
#		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#	)
#endif()

set(IO_SRC io.c)

add_library(${PROJECT_NAME} STATIC "${IO_SRC}")
include_directories(${PROJECT_SOURCE_DIR})

add_executable(example_stdio example_stdio.c)
target_link_libraries(example_stdio ${PROJECT_NAME})
add_test(NAME "example_stdio" COMMAND "${CMAKE_CURRENT_BINARY_DIR}/example_stdio")

add_executable(example_null example_null.c)
target_link_libraries(example_null ${PROJECT_NAME})
add_test(NAME "example_null" COMMAND "${CMAKE_CURRENT_BINARY_DIR}/example_null")
