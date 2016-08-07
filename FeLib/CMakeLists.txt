file(GLOB FELIB_SOURCES Include/*.h Source/*.cpp)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_definitions(-DGCC) # TODO: Use __GNUC__ in the code instead.
endif()

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

target_include_directories(FeLib PUBLIC Include)
target_include_directories(FeLib SYSTEM PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(FeLib ${SDL2_LIBRARY})
