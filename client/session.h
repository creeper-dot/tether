// client 会话
//
// 生命周期：
//   1. 收发 Hello，检查版本和能力
//   2. 发 StreamConfig（codec、码率、是否要声音）
//   3. 收 DisplayLayout，按远端尺寸创建窗口
//   4. 收到 UdpOffer 就尝试连 QUIC，成功后 attach 到 HybridTransport
//   5. 主循环，直到关窗口或连接断开
//
// 线程：
//   接收线程：recv -> 按 type 分发
//     VideoFrame  -> decoder -> 放进 ThreadQueue<DecodedFrame>(1)（只留最新一帧）
//                    解码失败 -> 发 RequestKeyframe
//     AudioPacket -> audio_player
//     CursorShape / CursorPos / Clipboard / DisplayLayout -> 放进事件队列，交给主线程
//   主线程（SDL 要求窗口操作在主线程）：
//     SDL_PollEvent -> input_capture -> transport.send
//     取最新帧 -> renderer.present -> 发 FrameAck
//     处理接收线程转过来的光标、剪贴板、布局变化
//     定时发 Ping 和 Stats
//
// 参考：
//   moonlight-qt/app/streaming/session.cpp       会话的完整生命周期，包括出错处理
//   tigervnc/vncviewer/CConn.cxx                 VNC 客户端连接：收到各种消息后怎么处理
//   tigervnc/vncviewer/DesktopWindow.cxx         窗口管理；第 345、393 行附近：窗口变化后请求远端改分辨率

#pragma once

#include <cstdint>
#include <memory>
#include <stop_token>
#include <thread>

import tether.protocol;
import tether.transport;

namespace tether::client {

struct SessionConfig {
    proto::Codec codec = proto::Codec::H264;
    std::uint32_t bitrate_kbps = 8000;
    std::uint16_t max_fps = 60;
    bool audio = true;
    bool clipboard = true;
    bool resize_remote = true;   // 窗口变化时请求远端改分辨率
    const char* host = nullptr;  // 连 QUIC 时用
};

class Session {
public:
    Session(std::unique_ptr<Transport> transport, const SessionConfig& cfg);
    ~Session();

    int run();

private:
    void recv_loop(std::stop_token st);

    std::unique_ptr<Transport> transport_;
    SessionConfig cfg_;

    // TODO: Decoder、SdlRenderer、InputCapture、CursorRenderer、ClipboardSync、
    //       AudioPlayer、ThreadQueue<DecodedFrame> 成员

    std::jthread receiver_;   // 放在成员最后：析构时最先 join，其他成员还活着
};

}  // namespace tether::client
