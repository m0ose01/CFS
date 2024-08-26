list(APPEND tests
	read_cfs_file
	get_general_header
	get_file_channel_header
	get_ds_general_header
	get_channel_data
	open_close_cfsfile
	get_file_variable
	)

list(APPEND descriptions
	"Read CFS file."
	"Get General File Info"
	"Get general header for channels 0 and 1"
	"Get DS general header for ds 0"
	"Get channel data."
	"Open and close a CFS file."
	"Get file variable 0"
	)

foreach(test description IN ZIP_LISTS tests descriptions)
	add_executable(${test} "${test}.c")

	target_include_directories(${test} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
	target_link_libraries(${test} cfsapi_static)

	add_test(NAME "${description}" COMMAND "${test}" "${CMAKE_CURRENT_SOURCE_DIR}/test_data/test_data.cfs")
endforeach()
