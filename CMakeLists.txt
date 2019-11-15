cmake_minimum_required(VERSION 2.8.12.2)
project(ivan CXX C)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 57)
set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
#set(VERSION_PATCH 0)
#set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
add_definitions(-DIVAN_VERSION="${PROJECT_VERSION}" -DUSE_SDL)

option(BUILD_MAC_APP "Build standalone application for MacOS" OFF)
option(USE_HOME_FOR_STATE_DIR "Statedir will be /.ivan/ in current user's homedir" OFF)
option(WIZARD "Enable Wizard Mode" OFF)

if(UNIX)
  add_definitions(-DUNIX)
  include(GNUInstallDirs)

  if(BUILD_MAC_APP)
    install(DIRECTORY Graphics Script Music Sound DESTINATION "ivan")
    install(FILES AUTHORS COPYING INSTALL LICENSING NEWS README README.md DESTINATION "docs")
    add_definitions(-DMAC_APP)
  else()
    install(DIRECTORY Graphics Script Music Sound DESTINATION "${CMAKE_INSTALL_DATADIR}/ivan")
    add_definitions(-DDATADIR="${CMAKE_INSTALL_FULL_DATADIR}")

    message("\n*NOTICE*\n"
            "    Highscore files and Bone files are now stored at \"\$HOME/.ivan\".\n"
            "    Your HOME directory is \"$ENV{HOME}\".\n")
    if(IS_DIRECTORY "${CMAKE_INSTALL_FULL_LOCALSTATEDIR}/ivan")
      message(WARNING
              "If you have game data in \"${CMAKE_INSTALL_FULL_LOCALSTATEDIR}/ivan\","
              " please move them to \"$ENV{HOME}/.ivan\".")
    endif()
  endif()

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
endif(UNIX)

# In visual studio 2017, add an option to hide the console window (useful where debugging is not intended - i.e. releases)
if(MSVC AND MSVC_HIDE_CONSOLE_WINDOW)
  # Hide the console window in visual studio projects
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

if(WIZARD)
  add_definitions(-DWIZARD)
endif()

find_path(EXECINFO_INCLUDE_DIR NAMES execinfo.h)
if(NOT EXECINFO_INCLUDE_DIR MATCHES "-NOTFOUND$")
  add_definitions(-DBACKTRACE)
endif()

add_subdirectory(FeLib)
add_subdirectory(audio)
add_subdirectory(Main)
add_subdirectory(igor)
add_subdirectory(mihail)
add_subdirectory(xbrzscale)
add_subdirectory(fantasyname)
