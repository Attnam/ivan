file(GLOB FANTASYNAME_SOURCES namegen.cc namegen.h)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_library(fantasyname ${FANTASYNAME_SOURCES})
