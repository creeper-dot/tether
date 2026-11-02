# 公共库：全部用 C++ 模块写
#
#   tether.util        错误类型、日志、计时、线程安全队列
#   tether.io          字节序列化、fd 读写、消息分帧（重新导出 tether.util）
#   tether.protocol    协议消息定义和编解码（重新导出 tether.io）
#   tether.transport   传输层接口、SSH(stdio) 实现、SSH+QUIC 组合（重新导出 tether.io）
#   tether.transport.quic   QUIC 实现，可选
#
# 文件约定：
#   xxx.cppm  模块接口单元（相当于头文件，写 export 的声明）
#   xxx.cpp   模块实现单元（开头写 module tether.xxx;，放函数实现）
#
# 一个模块拆成几个"分区"（partition），比如 tether.io:bytes、tether.io:fd，
# 主接口文件 io.cppm 用 export import :bytes; 把它们拼起来。
# 使用方只写 import tether.io;，不用关心分区。

add_library(tether_common STATIC)

target_sources(tether_common
  PUBLIC
    FILE_SET CXX_MODULES FILES
      util/util.cppm
      util/error.cppm
      util/log.cppm
      util/time.cppm
      util/thread_queue.cppm
      io/io.cppm
      io/bytes.cppm
      io/fd.cppm
      io/framing.cppm
      protocol/protocol.cppm
      transport/transport.cppm
      transport/base.cppm
      transport/stdio.cppm
      transport/hybrid.cppm
  PRIVATE
    util/error.cpp
    util/log.cpp
    io/framing.cpp
    protocol/protocol.cpp
    transport/stdio.cpp
    transport/hybrid.cpp
)

target_link_libraries(tether_common PUBLIC Threads::Threads)

# 平台相关实现：接口（.cppm）只有一份，实现按平台选
if(WIN32)
  target_sources(tether_common PRIVATE io/fd_win.cpp)
else()
  target_sources(tether_common PRIVATE io/fd_posix.cpp)
endif()

if(TETHER_WITH_QUIC)
  target_sources(tether_common
    PUBLIC FILE_SET CXX_MODULES FILES transport/quic.cppm
    PRIVATE transport/quic.cpp)
  target_include_directories(tether_common PRIVATE ${MSQUIC_INCLUDE})
  target_link_libraries(tether_common PRIVATE ${MSQUIC_LIB})
  target_compile_definitions(tether_common PUBLIC TETHER_HAVE_QUIC)
endif()
