// agent 会话：把各模块串起来
//
// 生命周期：
//   1. 收发 Hello，检查版本号和双方能力（Capability）
//   2. 收 StreamConfig，按 client 的要求创建 Capturer / Encoder / Injector
//   3. 发 DisplayLayout、AudioConfig
//   4. （可选）起 QUIC，发 UdpOffer
//   5. 启动各线程，直到连接断开
//   6. 清理：injector.release_all()、display.restore()
//
// 线程：
//   capture_loop          { flow.can_send()? -> grab -> encode -> send(Video) }
//   recv_loop             { recv -> 按 type 分发：输入给 injector，ack 给 flow，
//                           RequestKeyframe 给 encoder，ResizeRequest 给 display ... }
//   cursor_clipboard_loop { 每 10ms 查一次光标和剪贴板变化 -> send }
//   audio_loop            { 读音频 -> opus 编码 -> send(Audio) }
//
// 平台无关：截屏、注入等全部通过 Platform 创建（见 platform/platform.h），
// Session 里不出现任何 X11 / Windows / macOS 的东西。
// Linux 下 X11 / Wayland 的选择在 platform/linux/linux_platform.cpp 里
//
// 参考：
//   Sunshine/src/stream.cpp         会话怎么管理视频、音频、控制几条线
//   tigervnc/common/rfb/VNCSConnectionST.cxx   一个 VNC 连接的完整生命周期
//   FreeRDP/server/shadow/shadow_client.c      RDP 服务端每个客户端的处理流程

#pragma once
//
// 线程用 std::jthread：
//   std::jthread t([this](std::stop_token st) { capture_loop(st); });
//   循环里检查 st.stop_requested()；任一线程发现连接断开就调 stop_.request_stop()
//   所有线程共用 stop_ 这一个 std::stop_source，析构时 jthread 自动 join

#include <memory>
#include <stop_token>
#include <thread>
#include <vector>

#include "platform/platform.h"

import tether.transport;

namespace tether::agent {

struct SessionConfig {
    bool enable_quic = true;
};

class Session {
public:
    Session(std::unique_ptr<Transport> transport, std::unique_ptr<Platform> platform,
            const SessionConfig& cfg);
    ~Session();

    // 阻塞运行直到连接断开，返回进程退出码
    int run();

private:
    void capture_loop(std::stop_token st);
    void recv_loop(std::stop_token st);
    void cursor_clipboard_loop(std::stop_token st);
    void audio_loop(std::stop_token st);

    std::unique_ptr<Transport> transport_;
    std::unique_ptr<Platform> platform_;
    SessionConfig cfg_;
    std::stop_source stop_;

    // TODO: Capturer、Encoder、Injector、FlowControl、DisplayManager、
    //       CursorMonitor、Clipboard、AudioCapture、OpusEncoder 成员
    //       （平台相关的都用 platform_->create_xxx() 创建）

    std::vector<std::jthread> threads_;   // 放在成员最后：析构时最先 join，其他成员还活着
};

}  // namespace tether::agent
