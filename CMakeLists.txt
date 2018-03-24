cmake_minimum_required(VERSION 2.8.12.2)
project(ivan CXX C)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 52)
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
#set(VERSION_PATCH 0)
#set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
add_definitions(-DIVAN_VERSION="${PROJECT_VERSION}" -DUSE_SDL)

if(UNIX)
  add_definitions(-DUNIX)
  include(GNUInstallDirs)
  install(DIRECTORY Graphics Script Music Sound DESTINATION "${CMAKE_INSTALL_DATADIR}/ivan")
  add_definitions(-DDATADIR="${CMAKE_INSTALL_FULL_DATADIR}"
                  -DLOCAL_STATE_DIR="${CMAKE_INSTALL_FULL_LOCALSTATEDIR}/ivan")
elseif(WIN32)
  install(DIRECTORY Graphics Script Music Sound DESTINATION "ivan")
  install(FILES AUTHORS COPYING INSTALL LICENSING NEWS README README.md DESTINATION "ivan")
  add_definitions(-DWIN32)

if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS -mwindows)
endif(MINGW)

  # This is bad. To remove below defines, FeLib should switch to std::chrono and serialize time since common epoch.
  # 32 bit TIME_T has year 2038 problem. Moreover, serialized value is not portable for time_t in general.
  # Switching to std::chrono should not be hard, but it's not a topic for this pull request.
  add_definitions(-D_USE_32BIT_TIME_T)
endif()

add_subdirectory(FeLib)
add_subdirectory(audio)
add_subdirectory(Main)
add_subdirectory(igor)
add_subdirectory(mihail)
add_subdirectory(xbrzscale)
