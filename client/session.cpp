#include "session.h"

namespace tether::client {

Session::Session(std::unique_ptr<Transport> transport, const SessionConfig& cfg)
    : transport_(std::move(transport)), cfg_(cfg) {}

Session::~Session() {
    // TODO: receiver_.request_stop()，transport_->close() 让阻塞的 recv 醒来
}

int Session::run() {
    // TODO: 握手 -> 发 StreamConfig
    //       -> receiver_ = std::jthread([this](std::stop_token st) { recv_loop(st); });
    //       -> 主线程跑 SDL 事件循环
    return 0;
}

void Session::recv_loop(std::stop_token) {
    // TODO
}

}  // namespace tether::client
