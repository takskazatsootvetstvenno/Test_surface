cmake_minimum_required(VERSION 3.12)

SET(PROJECT_NAME Viewer)

get_filename_component(GENERAL_FOLDER ../ ABSOLUTE)
set(CMAKE_INSTALL_PREFIX "${GENERAL_FOLDER}" CACHE PATH "${GENERAL_FOLDER}" FORCE)

project(${PROJECT_NAME})

if(MSVC)
	 add_compile_options(/MP)
endif()

add_subdirectory(ViewerCore)