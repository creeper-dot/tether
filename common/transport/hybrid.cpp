// tether.transport:hybrid 的实现
module;

#include <memory>
#include <stop_token>
#include <utility>

module tether.transport;

namespace tether {

HybridTransport::HybridTransport(std::unique_ptr<Transport> ssh) : ssh_(std::move(ssh)) {
    // TODO: ssh_reader_ = std::jthread([this](std::stop_token st) { 循环 ssh_->recv() 推进 inbox_ });
}

HybridTransport::~HybridTransport() {
    // TODO: close()。jthread 成员析构时会自动 request_stop + join，
    //       但阻塞在 recv() 里的线程需要先 close() 才能醒过来
}

void HybridTransport::attach_quic(std::unique_ptr<Transport>) {
    // TODO: 加锁保存 quic_，启动 quic_reader_
}

void HybridTransport::detach_quic() {
    // TODO
}

bool HybridTransport::quic_active() const {
    // TODO
    return false;
}

Status HybridTransport::send(Channel, const Message&) {
    // TODO: ch != Control 且 QUIC 可用 -> 走 QUIC；QUIC 发送失败 -> detach 后退回 SSH
    return {};
}

Result<Message> HybridTransport::recv() {
    // TODO: auto r = inbox_.pop(); 队列关闭时返回 Closed
    return fail(Errc::Closed);
}

void HybridTransport::close() {
    // TODO: 关闭两条通道，inbox_.close()
}

}  // namespace tether
