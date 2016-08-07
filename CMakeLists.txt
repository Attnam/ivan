cmake_minimum_required(VERSION 3.0)

project(ivan VERSION 0.50.6)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
add_definitions(-DIVAN_VERSION="${PROJECT_VERSION}" -DUSE_SDL)

if(UNIX)
  add_definitions(-DUNIX)
  include(GNUInstallDirs)
  install(DIRECTORY Graphics Script DESTINATION "${CMAKE_INSTALL_DATADIR}/ivan")
  add_definitions(-DDATADIR="${CMAKE_INSTALL_FULL_DATADIR}"
                  -DLOCAL_STATE_DIR="${CMAKE_INSTALL_FULL_LOCALSTATEDIR}/ivan")
elseif(WIN32)
  add_definitions(-DWIN32)
endif()

add_subdirectory(FeLib)
add_subdirectory(Main)
add_subdirectory(igor)
add_subdirectory(mihail)
