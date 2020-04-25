file(GLOB FANTASYNAME_SOURCES namegen.cc namegen.h)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

if(HAVE_CXX14)
  add_definitions(-DHAVE_CXX14)
endif()

add_library(fantasyname ${FANTASYNAME_SOURCES})
