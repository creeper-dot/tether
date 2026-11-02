# 和平台无关的部分。平台相关的代码在 platform/<平台>/，由下面的 add_subdirectory 加进来
add_executable(tether-agent
  main.cpp
  session.cpp
  encode/raw_encoder.cpp
  flow/flow_control.cpp
)

target_include_directories(tether-agent PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tether-agent PRIVATE tether_common)

if(TETHER_WITH_FFMPEG)
  target_sources(tether-agent PRIVATE encode/ffmpeg_encoder.cpp)
  target_link_libraries(tether-agent PRIVATE PkgConfig::FFMPEG)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_FFMPEG)
endif()

# Opus 编码各平台通用；采集部分在各平台目录里
if(TETHER_WITH_AUDIO)
  target_sources(tether-agent PRIVATE audio/opus_encoder.cpp)
  target_link_libraries(tether-agent PRIVATE PkgConfig::OPUS)
  target_compile_definitions(tether-agent PRIVATE TETHER_HAVE_AUDIO)
endif()

if(TETHER_PLATFORM_LINUX)
  add_subdirectory(platform/linux)
elseif(TETHER_PLATFORM_WINDOWS)
  add_subdirectory(platform/windows)
elseif(TETHER_PLATFORM_MACOS)
  add_subdirectory(platform/macos)
endif()
