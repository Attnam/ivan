file(GLOB FEAUDIO_SOURCES *.h *.cpp)

add_library(FeAudio ${FEAUDIO_SOURCES})
target_link_libraries(FeAudio FeLib)

# Add RtMidi dependencies on Windows
if(WIN32)
  add_definitions(-D__WINDOWS_MM__)
  set(RTMIDI_DEPENDENCIES winmm)
endif()

# Add RtMidi dependencies on Linux
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  add_definitions(-D__LINUX_ALSA__)
  find_package(ALSA)
  set(RTMIDI_DEPENDENCIES ${ALSA_LIBRARY} pthread)
endif()

# Add RtMidi dependencies on Mac
if(APPLE)
  add_definitions(-D__MACOSX_CORE__)
  find_library(COREMIDI_LIBRARY CoreMIDI)
  find_library(COREAUDIO_LIBRARY CoreAudio)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)
  set(RTMIDI_DEPENDENCIES ${COREMIDI_LIBRARY} ${COREAUDIO_LIBRARY} ${COREFOUNDATION_LIBRARY})
endif(APPLE)

target_include_directories(FeAudio PUBLIC ../FeLib/Include ../Main/Include)
target_link_libraries(FeAudio ${RTMIDI_DEPENDENCIES})

find_package(SDL2 REQUIRED)

target_include_directories(FeAudio SYSTEM PUBLIC ${SDL2_mixer_INCLUDE_DIR})
target_link_libraries(FeAudio ${SDL2_mixer_LIBRARY})
