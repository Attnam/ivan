set(MIHAIL_VERSION 0.91)

add_executable(mihail EXCLUDE_FROM_ALL Source/mihail.cpp)
target_include_directories(mihail PUBLIC ../Felib/Include)
target_link_libraries(mihail FeLib xbrzscale)
