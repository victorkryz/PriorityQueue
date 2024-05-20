cmake_minimum_required(VERSION 3.7)
project(PriorityQueue)

set(CMAKE_CXX_STANDARD 20)

set(CURRENT_DIR  "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES_DIR "${CURRENT_DIR}/src")

message(status gggggg${SOURCE_FILES_DIR})

include_directories("${SOURCE_FILES_DIR}" "${SOURCE_FILES_DIR}/Server")

aux_source_directory("${SOURCE_FILES_DIR}" src_files)
aux_source_directory("${SOURCE_FILES_DIR}/Channel" src_files)
aux_source_directory("${SOURCE_FILES_DIR}/Client" src_files)
aux_source_directory("${SOURCE_FILES_DIR}/Server" src_files)


if(UNIX)
	set(EXTRA_LIBS pthread stdc++fs)
else()
	set(EXTRA_LIBS "")
endif()

add_compile_options(-fmodules-ts)

add_executable(PriorityQueue ${src_files})

target_link_libraries(PriorityQueue ${EXTRA_LIBS})
