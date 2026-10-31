// tether-client：控制端
//
// 用法（计划）：
//   tether-client user@host              通过 ssh 启动远端 tether-agent
//   tether-client --local ./tether-agent 本地直接启动 agent，调试用（不经过 ssh）
//
// 参数（计划）：
//   --agent-path PATH    远端 agent 路径，默认 tether-agent（要在远端 PATH 里）
//   --codec h264|hevc|raw
//   --bitrate 8000       kbps
//   --no-quic            全部走 SSH
//   --no-audio
//   -- 后面的参数原样传给 ssh，比如 -- -p 2222 -J jumphost
//
// 参考：
//   tigervnc/vncviewer/vncviewer.cxx    main：参数解析、连接、主循环
//   moonlight-qt/app/cli/               命令行模式的参数设计

#include <cstdio>

#include "session.h"

int main(int, char**) {
    // TODO:
    //   1. 解析参数
    //   2. tether::ignore_sigpipe()、tether::Logger::init_from_env()
    //   3. SshProcess.start({"ssh", host, "tether-agent", "--stdio"})
    //   4. StdioTransport(ssh.read_fd(), ssh.write_fd()) -> HybridTransport -> Session，run()
    fprintf(stderr, "tether-client: not implemented yet\n");
    return 1;
}
