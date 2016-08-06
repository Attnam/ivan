file(GLOB IVAN_HEADERS Include/*.h)
set(IVAN_SOURCES
    Source/actset.cpp
    Source/areaset.cpp
    Source/charset.cpp
    Source/charsset.cpp
    Source/command.cpp
    Source/coreset.cpp
    Source/dataset.cpp
    Source/dungeon.cpp
    Source/game.cpp
    Source/godset.cpp
    Source/iconf.cpp
    Source/id.cpp
    Source/igraph.cpp
    Source/itemset.cpp
    Source/levelset.cpp
    Source/main.cpp
    Source/materset.cpp
    Source/message.cpp
    Source/object.cpp
    Source/roomset.cpp
    Source/script.cpp
    Source/slotset.cpp
    Source/trapset.cpp
    Source/wmapset.cpp
    Source/wskill.cpp)

add_executable(ivan ${IVAN_HEADERS} ${IVAN_SOURCES})
target_include_directories(ivan PUBLIC Include ../Felib/Include)
target_link_libraries(ivan FeLib)
