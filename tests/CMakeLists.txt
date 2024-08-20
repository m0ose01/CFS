list(APPEND tests
	test_read_cfs_file
	test_read_general_header
	)

list(APPEND descriptions
	"Read CFS file."
	"Read General File Info"
	)

foreach(test description IN ZIP_LISTS tests descriptions)
	add_executable(${test} "${test}.c")

	target_include_directories(${test} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
	target_link_libraries(${test} cfsapi_static)

	add_test(NAME "${description}" COMMAND "${test}" "${CMAKE_CURRENT_SOURCE_DIR}/test_data/test_data.cfs")
endforeach()
