set(IGOR_VERSION 1.203)

add_executable(igor EXCLUDE_FROM_ALL Source/igor.cpp)
target_include_directories(igor PUBLIC ../Felib/Include)
target_include_directories(igor PUBLIC ../xbrzscale ../xbrzscale/xbrz)
target_link_libraries(igor FeLib)
