file(GLOB FELIB_SOURCES Include/*.h Source/*.cpp)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_definitions(-DGCC) # TODO: Use __GNUC__ in the code instead.
endif()

cmake_policy(SET CMP0004 OLD)
add_library(FeLib ${FELIB_SOURCES})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
find_package(SDL2)
if(SDL2_FOUND)
  message(STATUS "Using SDL2")
else()
  message(WARNING "SDL2 not found")
  find_package(SDL 1.2.0 REQUIRED)
  message(STATUS "Using SDL1")
endif()

if(MSVC)
  # Not very pretty solution. This finds SDL2.dll from SDL2.lib path, so that it can be installed where ivan.exe will end up.
  message("SDL2_LIBRARY ${SDL2_LIBRARY}")
  list(GET SDL2_LIBRARY 0 sdl_lib_dir)
  message("sdl_lib_dir ${sdl_lib_dir}")
  get_filename_component(sdl_lib_dir "${sdl_lib_dir}" DIRECTORY)
  message("sdl_lib_dir ${sdl_lib_dir}")
  install(FILES "${sdl_lib_dir}/SDL2.dll" DESTINATION "ivan")
endif()

if(MINGW)
  # Not very pretty solution. This finds SDL2.dll from SDL2.lib path, so that it can be installed where ivan.exe will end up.
  message("SDL2_LIBRARY ${SDL2_LIBRARY}")
  list(GET SDL2_LIBRARY 2 sdl_lib_dir)
  message("sdl_lib_dir ${sdl_lib_dir}")
  get_filename_component(sdl_lib_dir "${sdl_lib_dir}" DIRECTORY)
  message("sdl_lib_dir ${sdl_lib_dir}")
#  install(FILES "${sdl_lib_dir}/../bin/SDL2.dll" DESTINATION "ivan")

#  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
  if((NOT BUILD_SHARED_LIBS) AND BUILD_STATIC_LIBS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#    set(CMAKE_CXX_FLAGS "pkg-config --cflags sdl2 ${CMAKE_CXX_FLAGS}")
    set(SDL2_LIBRARIES "${SDL2_LIBRARIES} ${SDL2_LDFLAGS}")
  else()
    install(FILES "${sdl_lib_dir}/../bin/SDL2.dll" DESTINATION "ivan")
  endif()
endif()

# Try finding libpng using pkg-config first, because find_package(PNG) finds a
# mismatching header + library combination for me.
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PNG libpng)
  set(PNG_LIBRARIES ${PNG_LDFLAGS})
endif()
if(NOT PNG_FOUND)
  find_package(PNG REQUIRED)
  add_definitions(${PNG_DEFINITIONS})
endif()

target_include_directories(FeLib PUBLIC Include)
target_include_directories(FeLib SYSTEM PUBLIC ${SDL2_INCLUDE_DIR} PRIVATE ${PNG_INCLUDE_DIRS})
target_link_libraries(FeLib ${PNG_LIBRARIES} ${SDL2_LIBRARIES} -static-libgcc -static-libstdc++)
#target_link_libraries(FeLib ${SDL2_LIBRARIES} ${PNG_LIBRARIES} -lmingw32 -lSDL2main -lSDL2 -mwindows  -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc -static-libstdc++)
target_compile_options(FeLib PUBLIC ${SDL2_CFLAGS} ${SDL2_CFLAGS_OTHER})

