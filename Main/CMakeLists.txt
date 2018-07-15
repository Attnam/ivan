include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PCRE libpcre)
  set(PCRE_LIBRARIES ${PCRE_LDFLAGS})
endif()
if(NOT PCRE_FOUND)
  find_package(PCRE REQUIRED)
  add_definitions(${PCRE_DEFINITIONS})
endif()

find_package(SDL2 REQUIRED)

file(GLOB IVAN_SOURCES Include/*.h Source/*.cpp)
# The following files are #included from other .cpp files and so shouldn't be
# compiled directly. Setting the HEADER_FILE_ONLY property to TRUE prevents it.
set_source_files_properties(
    Source/action.cpp   Source/actions.cpp     Source/area.cpp     Source/bodypart.cpp
    Source/char.cpp     Source/cont.cpp        Source/database.cpp Source/entity.cpp
    Source/fluid.cpp    Source/gear.cpp        Source/god.cpp      Source/gods.cpp
    Source/human.cpp    Source/item.cpp        Source/level.cpp    Source/lsquare.cpp
    Source/lterra.cpp   Source/lterras.cpp     Source/materia.cpp  Source/materias.cpp
    Source/miscitem.cpp Source/nonhuman.cpp    Source/pool.cpp     Source/proto.cpp
    Source/rain.cpp     Source/room.cpp        Source/rooms.cpp    Source/slot.cpp
    Source/smoke.cpp    Source/square.cpp      Source/stack.cpp    Source/team.cpp
    Source/terra.cpp    Source/trap.cpp        Source/traps.cpp    Source/worldmap.cpp
    Source/wsquare.cpp  Source/wterra.cpp      Source/wterras.cpp  Source/hiteffect.cpp
    Source/cmdcraft.cpp Source/cmdswapweap.cpp 
    PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(ivan ${IVAN_SOURCES} Resource/Ivan.rc)
target_include_directories(ivan PUBLIC Include ../Felib/Include ../audio ../fantasyname ${PCRE_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIR})
target_link_libraries(ivan FeLib FeAudio xbrzscale fantasyname ${PCRE_LIBRARIES} ${SDL2_mixer_LIBRARY})

if(UNIX)
    install(TARGETS ivan DESTINATION "${CMAKE_INSTALL_BINDIR}")
else()
    install(TARGETS ivan DESTINATION "ivan")
endif()
