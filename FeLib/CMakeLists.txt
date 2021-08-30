include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PCRE libpcre)
  set(PCRE_LIBRARIES ${PCRE_LDFLAGS})
endif()
if(NOT PCRE_FOUND)
  if(MSVC)
    find_path(PCRE_INCLUDE_DIR NAMES pcre.h)
    find_library(PCRE_LIBRARY pcre)
  endif()
  find_package(PCRE REQUIRED)
  add_definitions(${PCRE_DEFINITIONS})
endif()

file(GLOB FELIB_SOURCES Include/*.h Source/*.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_definitions(-DGCC) # TODO: Use __GNUC__ in the code instead.
endif()

option(IGNORE_EXTRA_WHITESPACES "Sets CMP0004 to OLD" OFF)

if(IGNORE_EXTRA_WHITESPACES)
  cmake_policy(SET CMP0004 OLD)
endif()

add_library(FeLib ${FELIB_SOURCES})

find_package(SDL2 REQUIRED)

# Try finding libpng using pkg-config first, because find_package(PNG) finds a
# mismatching header + library combination for me.
include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PNG libpng)
  set(PNG_LIBRARIES ${PNG_LDFLAGS})
endif()
if(NOT PNG_FOUND)
  find_package(PNG REQUIRED)
  add_definitions(${PNG_DEFINITIONS})
endif()

if(MINGW)
  # Not very pretty solution. This finds SDL2.dll from SDL2.lib path, so that it can be installed where ivan.exe will end up.
  message("SDL2_LIBRARY ${SDL2_LIBRARY}")
  list(GET SDL2_LIBRARY 2 sdl_lib_dir)
  message("sdl_lib_dir ${sdl_lib_dir}")
  get_filename_component(sdl_lib_dir "${sdl_lib_dir}" DIRECTORY)
  message("sdl_lib_dir ${sdl_lib_dir}")
  get_filename_component(deps_path "${sdl_lib_dir}" DIRECTORY)

  if((NOT BUILD_SHARED_LIBS) AND BUILD_STATIC_LIBS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(SDL2_LIBRARIES ${SDL2_LIBRARY} ${SDL2_LDFLAGS})
  else()
    cmake_policy(SET CMP0087 NEW)
    set(IVAN_DEPENDENCY_PATHS "${deps_path}/bin/")
    message("IVAN_DEPENDENCY_PATHS ${IVAN_DEPENDENCY_PATHS}")

    # Transfer the value of ${IVAN_DEPENDENCY_PATHS} into the install script
    install(CODE "set(IVAN_DEPENDENCY_PATHS \"${IVAN_DEPENDENCY_PATHS}\")")

    string(REPLACE "/" "[\\\\/]" IVAN_DEPENDENCY_PATHS_FILTER ${IVAN_DEPENDENCY_PATHS})
    string(REPLACE ":" "\\\\:" IVAN_DEPENDENCY_PATHS_FILTER ${IVAN_DEPENDENCY_PATHS_FILTER})
    # message("IVAN_DEPENDENCY_PATHS_FILTER ${IVAN_DEPENDENCY_PATHS_FILTER}")

    LIST(APPEND post_include_regex ".*${IVAN_DEPENDENCY_PATHS_FILTER}*") # gnu dlls
    # message("post_include_regex ${post_include_regex}")
    # Transfer the value of ${post_include_regex} into the install script
    install(CODE "set(post_include_regex \"${post_include_regex}\")")

    install(CODE [[
      file(GET_RUNTIME_DEPENDENCIES
        LIBRARIES ${PNG_LIBRARIES} ${SDL2_LIBRARY} ${PCRE_LIBRARIES} ${SDL2_mixer_LIBRARY}
        EXECUTABLES $<TARGET_FILE:ivan>
        RESOLVED_DEPENDENCIES_VAR _r_deps
        UNRESOLVED_DEPENDENCIES_VAR _u_deps
        DIRECTORIES ${IVAN_DEPENDENCY_PATHS}
        POST_EXCLUDE_REGEXES "^.*\\.dll$" # First exclude all dlls
        POST_INCLUDE_REGEXES ${post_include_regex} # Thereafter include only the desired dlls
      )
      foreach(_file ${_r_deps})
        file(INSTALL
          DESTINATION "${CMAKE_INSTALL_PREFIX}/ivan"
          TYPE SHARED_LIBRARY
          FOLLOW_SYMLINK_CHAIN
          FILES "${_file}"
        )
      endforeach()
      list(LENGTH _u_deps _u_length)
      if("${_u_length}" GREATER 0)
        message(WARNING "Unresolved dependencies detected!")
        # message("Unresolved dependencies include: ${_u_deps}")
      endif()
    ]])
  endif() # if STATIC
endif() # if MINGW

if(MSVC AND _VCPKG_INSTALLED_DIR)
  # Gum solution for manually copying dynamic link libraries from the vcpkg bin directory
  install(FILES "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/libpng16.dll" DESTINATION "ivan")
  install(FILES "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/zlib1.dll" DESTINATION "ivan")
  install(FILES "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/SDL2.dll" DESTINATION "ivan")
  install(FILES "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/SDL2_mixer.dll" DESTINATION "ivan")
endif()

target_include_directories(FeLib PUBLIC Include)
target_include_directories(FeLib SYSTEM PUBLIC ${SDL2_INCLUDE_DIR} PRIVATE ${PNG_INCLUDE_DIRS} ${PCRE_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIR})
target_include_directories(FeLib PUBLIC ../xbrzscale ../xbrzscale/xbrz)

if((NOT BUILD_SHARED_LIBS) AND BUILD_STATIC_LIBS)
  target_link_libraries(FeLib ${PNG_LIBRARIES} ${SDL2_LIBRARIES} ${PCRE_LIBRARIES} ${SDL2_mixer_LIBRARY} -static-libgcc -static-libstdc++)
else()
  target_link_libraries(FeLib ${PNG_LIBRARIES} ${SDL2_LIBRARY} ${PCRE_LIBRARIES} ${SDL2_mixer_LIBRARY})
endif()

target_compile_options(FeLib PUBLIC ${SDL2_CFLAGS} ${SDL2_CFLAGS_OTHER})

