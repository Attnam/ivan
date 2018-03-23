file(GLOB IVAN_SOURCES Include/*.h Source/*.cpp)
# The following files are #included from other .cpp files and so shouldn't be
# compiled directly. Setting the HEADER_FILE_ONLY property to TRUE prevents it.
set_source_files_properties(
    Source/action.cpp Source/actions.cpp Source/area.cpp Source/bodypart.cpp
    Source/char.cpp Source/cont.cpp Source/database.cpp Source/entity.cpp
    Source/fluid.cpp Source/gear.cpp Source/god.cpp Source/gods.cpp
    Source/human.cpp Source/item.cpp Source/level.cpp Source/lsquare.cpp
    Source/lterra.cpp Source/lterras.cpp Source/materia.cpp Source/materias.cpp
    Source/miscitem.cpp Source/nonhuman.cpp Source/pool.cpp Source/proto.cpp
    Source/rain.cpp Source/room.cpp Source/rooms.cpp Source/slot.cpp
    Source/smoke.cpp Source/square.cpp Source/stack.cpp Source/team.cpp
    Source/terra.cpp Source/trap.cpp Source/traps.cpp Source/worldmap.cpp
    Source/wsquare.cpp Source/wterra.cpp Source/wterras.cpp
    PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(ivan ${IVAN_SOURCES} Resource/Ivan.rc)
target_include_directories(ivan PUBLIC Include ../Felib/Include ../audio)
target_link_libraries(ivan FeLib FeAudio SDL2_mixer pcre xbrzscale)

if(UNIX)
    install(TARGETS ivan DESTINATION "${CMAKE_INSTALL_BINDIR}")
else()
    install(TARGETS ivan DESTINATION "ivan")
endif()
