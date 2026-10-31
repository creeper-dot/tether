add_executable(tether-client
  main.cpp
  session.cpp
  ssh/ssh_process.cpp
  decode/raw_decoder.cpp
  render/sdl_renderer.cpp
  input/input_capture.cpp
  input/keymap.cpp
  cursor/cursor_renderer.cpp
  clipboard/clipboard_sync.cpp
)

target_include_directories(tether-client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL2 REQUIRED)
target_link_libraries(tether-client PRIVATE tether_common SDL2::SDL2)

if(TETHER_WITH_FFMPEG)
  target_sources(tether-client PRIVATE decode/ffmpeg_decoder.cpp)
  target_link_libraries(tether-client PRIVATE PkgConfig::FFMPEG)
  target_compile_definitions(tether-client PRIVATE TETHER_HAVE_FFMPEG)
endif()

if(TETHER_WITH_AUDIO)
  target_sources(tether-client PRIVATE audio/audio_player.cpp)
  target_link_libraries(tether-client PRIVATE PkgConfig::AUDIO)
  target_compile_definitions(tether-client PRIVATE TETHER_HAVE_AUDIO)
endif()
