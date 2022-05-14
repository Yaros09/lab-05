cmake_minimum_required(VERSION 3.4)
SET(COVERAGE OFF CACHE BOOL "Coverage")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(Work_Gtest)

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest" 
	"gtest"
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/banking
)

add_executable(
	testing 
	${CMAKE_CURRENT_SOURCE_DIR}/tests/testing.cpp
)

target_link_libraries(
	testing PRIVATE
	gtest gtest_main gmock_main
)

if(COVERAGE)
	target_compile_options(testing PRIVATE --coverage)
	target_link_libraries(testing PRIVATE --coverage)
endif()

target_include_directories(
	testing PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/banking
)

target_link_libraries(
	testing PRIVATE
	banking
)

target_compile_options(
	testing PRIVATE
	-Wall -Wextra -pedantic -Werror
)