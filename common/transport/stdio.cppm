// tether.transport:stdio  基于一对管道句柄（NativeHandle）的传输层
//
// agent 端：in = std_input()，out = std_output()，由 sshd 接到 client
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
    StdioTransport(NativeHandle in, NativeHandle out);
    ~StdioTransport() override;

    Status send(Channel ch, const Message& msg) override;
    Result<Message> recv() override;
    void close() override;

private:
    NativeHandle in_fd_;
    NativeHandle out_fd_;
    FrameReader reader_;
    FrameWriter writer_;
};

}  // namespace tether
