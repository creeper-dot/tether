// tether-agent：被控端
//
// 由 client 通过 ssh 启动：ssh host tether-agent --stdio
// stdin/stdout 是协议通道，日志只能写 stderr（ssh 会把 stderr 原样转到 client 的终端）
//
// 参数（计划）：
//   --stdio            必须，表示协议走 stdin/stdout
//   --display :0       X11 display，默认用 $DISPLAY（只对 Linux 有效）
//   --backend x11|wayland   默认按 $XDG_SESSION_TYPE 自动选（只对 Linux 有效）
//   --no-quic          不开 QUIC，全部走 SSH
//
// 参考：tigervnc/unix/x0vncserver/x0vncserver.cxx（main 函数：参数解析、初始化、主循环）

#include <cstdio>

#include "session.h"

int main(int, char**) {
    // TODO:
    //   1. 解析参数
    //   2. tether::ignore_sigpipe()、tether::Logger::init_from_env()
    //   3. create_platform(PlatformOptions{...})，失败就打日志退出
    //   4. 创建 StdioTransport(tether::std_input(), tether::std_output())
    //      -> 包进 HybridTransport -> 和 platform 一起交给 Session，run()
    fprintf(stderr, "tether-agent: not implemented yet\n");
    return 1;
}
