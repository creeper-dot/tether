# macOS 后端：ScreenCaptureKit、CGEvent、NSPasteboard
#
# .mm 是 Objective-C++，只负责调 Objective-C API，不使用 C++ 模块（见 objc_bridge.h）
# OBJCXX 语言在根目录 CMakeLists.txt 里启用（enable_language 要在使用它的 target 所在目录或更上层调用）

target_sources(tether-agent PRIVATE
  macos_platform.cpp
  keymap.cpp
  sck_capture.cpp
  cgevent_injector.cpp
  mac_cursor_monitor.cpp
  mac_clipboard.cpp
  mac_display.cpp
  sck_stream.mm
  pasteboard.mm
  ns_cursor.mm)

if(TETHER_WITH_AUDIO)
  target_sources(tether-agent PRIVATE sck_audio_capture.cpp)
endif()

# .mm 用 ARC（自动管理 Objective-C 对象的引用计数）
set_source_files_properties(sck_stream.mm pasteboard.mm ns_cursor.mm PROPERTIES
  COMPILE_OPTIONS "-fobjc-arc")

target_link_libraries(tether-agent PRIVATE
  "-framework ScreenCaptureKit"
  "-framework CoreGraphics"
  "-framework CoreMedia"
  "-framework CoreVideo"
  "-framework ApplicationServices"
  "-framework AppKit"
  "-framework Carbon")
