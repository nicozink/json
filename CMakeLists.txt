cmake_minimum_required(VERSION 3.0)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build_tools/cmake/cmake_utils.cmake)
	include(${CMAKE_CURRENT_SOURCE_DIR}/build_tools/cmake/cmake_utils.cmake)
endif()

PROJECT(json_tests VERSION 1.0.0)

enable_testing()

set(SRC_MAIN
	${CMAKE_CURRENT_SOURCE_DIR}/include/json/json.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/json.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/json_lexer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/json_lexer.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/json_parser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/json_parser.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/json_token.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/json_value.cpp)

set(SRC_TEST
	${CMAKE_CURRENT_SOURCE_DIR}/test/test_parse.cpp)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/")

foreach(_source IN ITEMS ${SRC_MAIN} ${SRC_TEST})
	get_filename_component(_source_path "${_source}" PATH)
	file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
	string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
	source_group("${_group_path}" FILES "${_source}")
endforeach()

create_library(json library ${SRC_MAIN})

target_include_directories(json
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(json PRIVATE cxx_std_17)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libraries/unittest/CMakeLists.txt)
	add_subdirectory(libraries/unittest)
endif()

create_library(json_tests tests ${SRC_TEST})

target_link_libraries(json_tests json unittest)

create_executable(json_test test ${SRC_TEST})

target_link_libraries(json_test json unittest)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	add_test(NAME json_test COMMAND ${NODE_JS} "${CMAKE_CURRENT_BINARY_DIR}/json_test.js")
else ()
	add_test(NAME json_test COMMAND json_test)
endif ()
