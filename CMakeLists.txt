cmake_minimum_required(VERSION 2.6)
project(libra)
find_program(GIT_PATH git)
set(PROJECT_SOURCE_DIR src)
IF(${GIT_PATH} STREQUAL "GIT_PATH-NOTFOUND")
	set(GIT_VERSION "unknown")
ELSE()
	EXEC_PROGRAM("${GIT_PATH} describe --tags" OUTPUT_VARIABLE GIT_VERSION CACHE STRING)
ENDIF()

SET(GIT_VERSION "${GIT_VERSION}")
site_name(HOST_NAME)
configure_file (
	"${PROJECT_SOURCE_DIR}/Config.h.in"
	"${PROJECT_SOURCE_DIR}/Config.h"
)
file(GLOB_RECURSE LIBRA_SOURCE "src/*.cpp")
file(GLOB_RECURSE LIBRA_HEADERS "src/*.hpp")
add_executable(libra ${LIBRA_SOURCE})
find_package(Qt COMPONENTS QtCore QtGui REQUIRED)
if(QT_FOUND)
	include_directories(${QT_INCLUDE_DIRS})
	target_link_libraries(libra ${QT_LIBRARIES})
endif(QT_FOUND)
