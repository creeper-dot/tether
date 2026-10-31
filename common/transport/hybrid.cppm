// tether.transport:hybrid  组合传输：SSH 通道 + 可选的 QUIC 通道
//
// 连接流程（参考 mosh 的设计：https://mosh.org/mosh-paper.pdf 第 2 节）：
//   1. 先只有 SSH（StdioTransport），握手
//   2. agent 起 QUIC 服务，通过 SSH 发 UdpOffer（端口、证书指纹、一次性令牌）
//   3. client 连 QUIC，校验指纹，发令牌；成功后调 attach_quic()
//   4. 之后 Control 仍走 SSH，Video/Audio/Input 走 QUIC
//   5. QUIC 超时或断开 -> detach_quic()，所有消息退回 SSH，会话不断
//
// 两条通道各有一个接收线程（std::jthread），收到的消息合并进 inbox_，recv() 从里面取。
//
// 要点：
//   - 令牌和指纹只通过 SSH 通道传，这是 QUIC 通道安全性的来源
//   - QUIC 断开不等于会话结束；SSH 断开才是
//   - inbox_ 里放 Result<Message>，SSH 通道的错误（包括 Closed）也要传给 recv()

module;

#include <memory>
#include <mutex>
#include <thread>

export module tether.transport:hybrid;

import tether.io;
import :base;

export namespace tether {

class HybridTransport final : public Transport {
public:
    explicit HybridTransport(std::unique_ptr<Transport> ssh);
    ~HybridTransport() override;

    // 线程安全，可以在会话进行中调用
    void attach_quic(std::unique_ptr<Transport> quic);
    void detach_quic();
    bool quic_active() const;

    Status send(Channel ch, const Message& msg) override;
    Result<Message> recv() override;
    void close() override;

private:
    std::unique_ptr<Transport> ssh_;
    std::unique_ptr<Transport> quic_;
    mutable std::mutex mutex_;
    ThreadQueue<Result<Message>> inbox_;
    std::jthread ssh_reader_;
    std::jthread quic_reader_;
};

}  // namespace tether
