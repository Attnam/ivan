file(GLOB XBRZSCALE_SOURCES *.h libxbrzscale.cpp xbrz/*.h xbrz/*.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -DNDEBUG ${CMAKE_CXX_FLAGS}")

add_library(xbrzscale ${XBRZSCALE_SOURCES})
target_include_directories(xbrzscale PUBLIC xbrz)
target_include_directories(xbrzscale SYSTEM PUBLIC ${SDL2_INCLUDE_DIR})
