file(GLOB FASTNOISE_SOURCES FastNoise.cpp FastNoise.h)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

if(HAVE_CXX14)
  add_definitions(-DHAVE_CXX14)
endif()

add_library(fastnoise ${FASTNOISE_SOURCES})
