cmake_minimum_required(VERSION 3.22)
project(
	CFS VERSION 0.0.1
	DESCRIPTION "A library which re-implements a subset of the CFS API"
	LANGUAGES C
	)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_executable(cfstocsv src/cfstocsv.c src/cfs.c)
add_library(cfsapi SHARED src/cfsapi.c src/cfs.c)
add_library(cfsapi_static STATIC src/cfsapi.c src/cfs.c)

get_property(target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

foreach(current_target IN LISTS target_names)
	if(MSVC)
		target_compile_options(${current_target} PRIVATE /W3)
	else()
		target_compile_options(${current_target} PRIVATE -Wall -Wextra -Wpedantic)
	endif()
endforeach()

target_include_directories(cfstocsv PUBLIC include)
target_include_directories(cfsapi PUBLIC include)
target_include_directories(cfsapi_static PUBLIC include)

enable_testing()
add_subdirectory("tests")
