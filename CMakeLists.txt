cmake_minimum_required(VERSION 3.17)
project(stoneDB)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_CXX_STANDARD 17)

set (OCCI_LIB_NAME oci.lib)
link_directories(./external/oci/lib/msvc)

set(EXECUTABLE_OUTPUT_PATH "bin")

set (stoneDB_VERSION_MAJOR 0)
set (stoneDB_VERSION_MINOR 0)
set (stoneDB_VERSION_PATCH 1)

configure_file(
	"${PROJECT_SOURCE_DIR}/src/StoneDBConfig.h.in"
	"${PROJECT_BINARY_DIR}/src/StoneDBConfig.h"
)

include_directories(
	./external/oci/include
	"${PROJECT_BINARY_DIR}"
)

add_executable (stoneDB ${SOURCES})
target_link_libraries(stoneDB ${OCCI_LIB_NAME} ${CLNT_LIB_NAME} ${PTHD_LIB_NAME} )
