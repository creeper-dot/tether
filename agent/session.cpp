#include "session.h"

namespace tether::agent {

Session::Session(std::unique_ptr<Transport> transport, std::unique_ptr<Platform> platform,
                 const SessionConfig& cfg)
    : transport_(std::move(transport)), platform_(std::move(platform)), cfg_(cfg) {}

Session::~Session() {
    // TODO: stop_.request_stop()，transport_->close() 让阻塞的 recv 醒来
}

int Session::run() {
    // TODO: 握手 -> 创建各模块 -> 启动各线程（threads_.emplace_back(...)）
    //       -> 当前线程跑 recv_loop(stop_.get_token()) -> 清理
    return 0;
}

void Session::capture_loop(std::stop_token) {
    // TODO
}

void Session::recv_loop(std::stop_token) {
    // TODO
}

void Session::cursor_clipboard_loop(std::stop_token) {
    // TODO
}

void Session::audio_loop(std::stop_token) {
    // TODO
}

}  // namespace tether::agent
