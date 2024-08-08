cmake_minimum_required(VERSION 3.22)
project(
	CFS VERSION 0.0.1
	DESCRIPTION "A library which re-implements a subset of the CFS API"
	LANGUAGES C
	)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cfstocsv src/cfstocsv.c src/cfs.c)
add_library(cfsapi SHARED src/cfsapi.c src/cfs.c)

target_include_directories(cfstocsv PUBLIC include)
target_include_directories(cfsapi PUBLIC include)
