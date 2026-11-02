# Windows 后端：DXGI 截屏、SendInput、WASAPI

target_sources(tether-agent PRIVATE
  windows_platform.cpp
  win_util.cpp
  keymap.cpp
  dxgi_capture.cpp
  sendinput_injector.cpp
  win_cursor_monitor.cpp
  win_clipboard.cpp
  win_display.cpp)

target_compile_definitions(tether-agent PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
# d3d11/dxgi 截屏，user32 注入/光标/剪贴板/显示器，gdi32 读光标位图
target_link_libraries(tether-agent PRIVATE d3d11 dxgi user32 gdi32)

if(TETHER_WITH_AUDIO)
  target_sources(tether-agent PRIVATE wasapi_audio_capture.cpp)
  target_link_libraries(tether-agent PRIVATE ole32 avrt)
endif()
