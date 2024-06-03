cmake_minimum_required(VERSION 3.7)
project(PriorityQueue)

set(CMAKE_CXX_STANDARD 17)

set(CURRENT_DIR  "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES_DIR "${CURRENT_DIR}/src")

include_directories("${SOURCE_FILES_DIR}" "${SOURCE_FILES_DIR}/Server")

aux_source_directory("${SOURCE_FILES_DIR}" main_src_files)
aux_source_directory("${SOURCE_FILES_DIR}/Channel" src_files)
aux_source_directory("${SOURCE_FILES_DIR}/Client" src_files)
aux_source_directory("${SOURCE_FILES_DIR}/Server" src_files)
aux_source_directory("${SOURCE_FILES_DIR}/Utils" src_files)


if(UNIX)
	set(EXTRA_LIBS pthread)
else()
	set(EXTRA_LIBS "")
endif()

set(CLIENT_SERVER_LIB ClientServerImpl)

add_library(
    ${CLIENT_SERVER_LIB}
    ${src_files}
)

add_executable(${CMAKE_PROJECT_NAME} ${main_src_files})

target_link_libraries(${CMAKE_PROJECT_NAME} ${CLIENT_SERVER_LIB} ${EXTRA_LIBS})

add_subdirectory(src/g.tests)
