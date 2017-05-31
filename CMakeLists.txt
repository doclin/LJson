cmake_minimum_required(VERSION 2.8)
PROJECT (LJSON)

include_directories("${PROJECT_SOURCE_DIR}/src")
add_subdirectory(src ${PROJECT_SOURCE_DIR}/bin/LJson_lib)
add_subdirectory(test ${PROJECT_SOURCE_DIR}/bin)