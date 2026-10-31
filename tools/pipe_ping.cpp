// 练习：两个进程通过管道互发 ping/pong
//
// 一个程序两种模式：
//   pipe_ping --server   从 stdin 收 Ping，往 stdout 回 Pong（模拟 agent）
//   pipe_ping --client   启动一个 "pipe_ping --server" 子进程，发 10 个 Ping，打印 RTT
//
// 跑通本地后，把 client 启动的命令换成：
//   ssh -T -e none localhost /path/to/pipe_ping --server
// 就验证了"经 SSH 的 stdin/stdout 传协议"这条路是通的。
//
// 用到：tether.transport（StdioTransport）、tether.protocol（Ping/Pong）、
//       tether.util（now_us、Logger）
// 启动子进程的代码写完后可以搬进 client/ssh/ssh_process.cpp

#include <cstdio>
#include <string_view>

import tether.protocol;
import tether.transport;

int main(int argc, char** argv) {
    std::string_view mode = argc >= 2 ? argv[1] : "";
    if (mode == "--server") {
        // TODO: StdioTransport t(0, 1);
        //       循环 t.recv()：收到 kPing 就把 type 改成 kPong 原样发回去；Errc::Closed 时退出
        return 0;
    }
    if (mode == "--client") {
        // TODO: 启动子进程，发 Ping(now_us())，收 Pong，打印 now_us() - timestamp
        return 0;
    }
    std::fprintf(stderr, "usage: %s --server | --client [ssh args...]\n", argv[0]);
    return 2;
}
