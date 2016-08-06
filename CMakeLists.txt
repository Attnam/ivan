cmake_minimum_required(VERSION 3.0)

project(ivan VERSION 0.50.6)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-DUSE_SDL)

if(NOT WIN32)
  add_definitions(-DLINUX) # FIXME: Not all non-Windows platforms are Linux.
endif()

include(GNUInstallDirs)
add_definitions(-DDATADIR="${CMAKE_INSTALL_FULL_DATADIR}"
                -DLOCAL_STATE_DIR="${CMAKE_INSTALL_FULL_LOCALSTATEDIR}")

add_subdirectory(FeLib)
add_subdirectory(Main)
add_subdirectory(igor)
add_subdirectory(mihail)
