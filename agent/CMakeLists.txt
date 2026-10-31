add_executable(tether-agent
  main.cpp
  session.cpp
  capture/x11_capture.cpp
  display/display_manager.cpp
  encode/raw_encoder.cpp
  input/xtest_injector.cpp
  input/keymap.cpp
  cursor/cursor_monitor.cpp
  clipboard/clipboard.cpp
  flow/flow_control.cpp
)

target_include_directories(tether-agent PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(X11 REQUIRED)
target_link_libraries(tether-agent PRIVATE
  tether_common
  X11::X11 X11::Xext X11::Xtst X11::Xfixes X11::Xrandr X11::Xdamage)

if(TETHER_WITH_FFMPEG)
  target_sources(tether-agent PRIVATE encode/ffmpeg_encoder.cpp)
  target_link_libraries(tether-agent PRIVATE PkgConfig::FFMPEG)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_FFMPEG)
endif()

if(TETHER_WITH_UINPUT)
  target_sources(tether-agent PRIVATE input/uinput_injector.cpp)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_UINPUT)
endif()

if(TETHER_WITH_PIPEWIRE)
  target_sources(tether-agent PRIVATE
    portal/portal_session.cpp
    capture/pipewire_capture.cpp
    input/portal_injector.cpp)
  target_link_libraries(tether-agent PRIVATE PkgConfig::PIPEWIRE)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_PIPEWIRE)
endif()

if(TETHER_WITH_AUDIO)
  target_sources(tether-agent PRIVATE
    audio/audio_capture.cpp
    audio/opus_encoder.cpp)
  target_link_libraries(tether-agent PRIVATE PkgConfig::AUDIO)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_AUDIO)
endif()
