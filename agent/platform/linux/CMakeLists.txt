# Linux 后端：X11（默认）、Wayland（TETHER_WITH_PIPEWIRE）、uinput、PulseAudio

target_sources(tether-agent PRIVATE
  linux_platform.cpp
  keymap.cpp
  x11_capture.cpp
  xtest_injector.cpp
  x11_cursor_monitor.cpp
  x11_clipboard.cpp
  xrandr_display.cpp)

find_package(X11 REQUIRED)
target_link_libraries(tether-agent PRIVATE
  X11::X11 X11::Xext X11::Xtst X11::Xfixes X11::Xrandr X11::Xdamage)

if(TETHER_WITH_UINPUT)
  target_sources(tether-agent PRIVATE uinput_injector.cpp)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_UINPUT)
endif()

if(TETHER_WITH_PIPEWIRE)
  target_sources(tether-agent PRIVATE
    portal_session.cpp
    pipewire_capture.cpp
    portal_injector.cpp)
  target_link_libraries(tether-agent PRIVATE PkgConfig::PIPEWIRE)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_PIPEWIRE)
endif()

if(TETHER_WITH_AUDIO)
  pkg_check_modules(PULSE REQUIRED IMPORTED_TARGET libpulse-simple)
  target_sources(tether-agent PRIVATE pulse_audio_capture.cpp)
  target_link_libraries(tether-agent PRIVATE PkgConfig::PULSE)
endif()
