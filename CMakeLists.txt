cmake_minimum_required(VERSION 3.28)   # 3.28 起正式支持 C++20 模块
project(tether LANGUAGES CXX)
if(APPLE)
  enable_language(OBJCXX)              # macOS agent 的 .mm 文件（agent/platform/macos）
endif()

# ---------------------------------------------------------------------------
# 编译器：必须是 Clang 18+。
# GCC 14 在模块里引入标准库头文件时会出现 internal compiler error，暂时不能用。
# 推荐直接用预设：cmake --preset debug && cmake --build --preset debug
# ---------------------------------------------------------------------------
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18)
  message(FATAL_ERROR
    "需要 Clang 18 或更新版本（当前：${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}）。\n"
    "请用 cmake --preset debug，或者加 -DCMAKE_CXX_COMPILER=clang++-20")
endif()

# 模块需要 Ninja 1.11+（Makefile 生成器不支持模块）
if(NOT CMAKE_GENERATOR MATCHES "Ninja")
  message(FATAL_ERROR "C++ 模块需要 Ninja 生成器：cmake -G Ninja ...")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # 用 -std=c++23 而不是 -std=gnu++23
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # 给 clangd 用

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra)

# 框架阶段很多类的成员变量还没被用到，Clang 会报 unused-private-field。
# 各模块写完后删掉这一行，让编译器帮你检查真正多余的成员。
add_compile_options(-Wno-unused-private-field)

# Debug 下开 ASan/UBSan，越界、use-after-free 会直接报错并给出位置
option(TETHER_SANITIZE "Enable ASan/UBSan in Debug builds" ON)
if(TETHER_SANITIZE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# ---------------------------------------------------------------------------
# 目标平台。平台相关代码按这个选：
#   agent   agent/platform/linux|windows|macos/
#   client  client/ssh/ssh_process_posix.cpp | ssh_process_win.cpp
#   common  common/io/fd_posix.cpp | fd_win.cpp
# Windows / macOS 的 agent 后端目前只有空壳，能编译，运行时报 Unsupported。
# ---------------------------------------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(TETHER_PLATFORM_LINUX ON)
elseif(WIN32)
  set(TETHER_PLATFORM_WINDOWS ON)
elseif(APPLE)
  set(TETHER_PLATFORM_MACOS ON)
else()
  message(FATAL_ERROR "不支持的平台：${CMAKE_SYSTEM_NAME}")
endif()

# ---------------------------------------------------------------------------
# 可选功能。MVP 只需要默认打开的几项，其余等做到那一步再打开：
#   cmake --preset debug -DTETHER_WITH_AUDIO=ON
#
# 必需依赖（Ubuntu 包名）：
#   agent:  libx11-dev libxext-dev libxtst-dev libxfixes-dev libxrandr-dev libxdamage-dev
#   client: libsdl2-dev
# ---------------------------------------------------------------------------
include(CMakeDependentOption)
option(TETHER_WITH_FFMPEG   "H.264/HEVC 编解码 (libavcodec-dev libavutil-dev libswscale-dev)" ON)
# 下面两项只在 Linux 上可选，其他平台强制关闭
cmake_dependent_option(TETHER_WITH_UINPUT   "agent 用 /dev/uinput 注入输入（只需内核头文件）"
  ON  "TETHER_PLATFORM_LINUX" OFF)
cmake_dependent_option(TETHER_WITH_PIPEWIRE "Wayland 截屏和输入 (libpipewire-0.3-dev libglib2.0-dev)"
  OFF "TETHER_PLATFORM_LINUX" OFF)
option(TETHER_WITH_AUDIO    "声音传输 (libopus-dev libpulse-dev)"                            OFF)
option(TETHER_WITH_QUIC     "QUIC 数据通道 (libmsquic)"                                      OFF)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

if(TETHER_WITH_FFMPEG)
  pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavcodec libavutil libswscale)
endif()
if(TETHER_WITH_PIPEWIRE)
  pkg_check_modules(PIPEWIRE REQUIRED IMPORTED_TARGET libpipewire-0.3 gio-2.0)
endif()
if(TETHER_WITH_AUDIO)
  # Opus 各平台通用；Linux 的 PulseAudio 在 agent/platform/linux 里单独找
  pkg_check_modules(OPUS REQUIRED IMPORTED_TARGET opus)
endif()
if(TETHER_WITH_QUIC)
  # msquic 没有 pkg-config 文件，按实际安装位置调整
  find_library(MSQUIC_LIB msquic REQUIRED)
  find_path(MSQUIC_INCLUDE msquic.h REQUIRED)
endif()

add_subdirectory(common)   # 公共模块 tether.util / io / protocol / transport
add_subdirectory(agent)    # 被控端 tether-agent
add_subdirectory(client)   # 控制端 tether-client
add_subdirectory(tools)    # 独立小练习程序

enable_testing()
add_subdirectory(tests)
