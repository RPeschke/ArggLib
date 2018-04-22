# project name
PROJECT( ArggLib_tests)

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

 cmake_minimum_required(VERSION 3.0)
 
 SET(INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")


If(WIN32)
SET(ADDITIONAL_LIBRARIES C:/Users/Richard/Documents/github/vcpkg/installed/x86-windows/lib/*.lib)
add_compile_options("/std:c++latest")
else(WIN32)
#add_compile_options("-pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(WIN32)

ADD_SUBDIRECTORY(Tests)

ADD_SUBDIRECTORY(Tests_ROOT)

