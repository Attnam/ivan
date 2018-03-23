file(GLOB XBRZSCALE_SOURCES *.h *.cpp xbrz/*.h xbrz/*.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -DNDEBUG ${CMAKE_CXX_FLAGS}")

add_library(xbrzscale ${XBRZSCALE_SOURCES})
target_include_directories(xbrzscale PUBLIC xbrz)
