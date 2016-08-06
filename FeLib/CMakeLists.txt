file(GLOB FELIB_HEADERS Include/*.h)
file(GLOB FELIB_SOURCES Source/*.cpp)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_definitions(-DGCC) # TODO: Use __GNUC__ in the code instead.
endif()

add_library(FeLib ${FELIB_HEADERS} ${FELIB_SOURCES})

find_package(SDL2)
if(SDL2_FOUND)
  message(STATUS "Using SDL2")
else()
  message(WARNING "SDL2 not found")
  find_package(SDL 1.2.0 REQUIRED)
  message(STATUS "Using SDL1")
endif()

string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES) # Fix error about trailing whitespace.
target_include_directories(FeLib PUBLIC Include)
target_include_directories(FeLib SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(FeLib ${SDL2_LIBRARIES})
