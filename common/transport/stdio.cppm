// tether.transport:stdio  基于一对文件描述符的传输层
//
// agent 端：in_fd = 0 (stdin)，out_fd = 1 (stdout)，由 sshd 接到 client
// client 端：in_fd / out_fd 是 ssh 子进程的 stdout / stdin（见 client/ssh/ssh_process.h）
// 测试时：用 pipe() 造两对 fd 直接接起来，不需要 ssh
//
// 实现很薄：send 调 FrameWriter，recv 调 FrameReader。

module;

#include <memory>

export module tether.transport:stdio;

import tether.io;
import :base;

export namespace tether {

class StdioTransport final : public Transport {
public:
    StdioTransport(int in_fd, int out_fd);
    ~StdioTransport() override;

    Status send(Channel ch, const Message& msg) override;
    Result<Message> recv() override;
    void close() override;

private:
    int in_fd_;
    int out_fd_;
    FrameReader reader_;
    FrameWriter writer_;
};

}  // namespace tether
