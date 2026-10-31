// 流控：限制"已发出但 client 还没确认"的帧数，并估算 RTT 和带宽
//
// SSH 是 TCP，发得比网络快时数据会在缓冲区里越堆越多，延迟越来越大。
// 做法：client 每显示一帧回一个 FrameAck，agent 在途帧数超过上限就先不发新帧。
//
// 参考：
//   tigervnc/common/rfb/Congestion.cxx（503 行）
//     它的思路：用 ping 估算 RTT，用"在途字节数"估算拥塞窗口
//   tigervnc/common/rfb/VNCSConnectionST.cxx  搜 "congestion"，看它在发送前怎么判断
//   Sunshine/src/stream.cpp                   搜 "bitrate"，看它怎么根据丢包调码率
//
// 实现分两步：
//   第一步：只限制在途帧数（can_send / on_sent / on_ack），这就能解决延迟堆积
//   第二步：根据 RTT 和每帧大小估算可用带宽，用 suggested_bitrate_kbps() 调编码器码率
//
// 线程：采集线程调 can_send()/on_sent()，接收线程调 on_ack()，需要线程安全

#pragma once

#include <cstddef>
#include <cstdint>
#include <mutex>

namespace tether::agent {

class FlowControl {
public:
    explicit FlowControl(int max_in_flight = 2) : max_in_flight_(max_in_flight) {}

    bool can_send() const;
    void on_sent(std::uint64_t frame_id, std::size_t bytes);
    void on_ack(std::uint64_t frame_id);

    std::uint64_t rtt_us() const;
    int suggested_bitrate_kbps() const;   // 第二步再做，先返回 0 表示不建议调整

private:
    int max_in_flight_;
    mutable std::mutex mutex_;
    std::uint64_t last_sent_ = 0;
    std::uint64_t last_acked_ = 0;
    std::uint64_t rtt_us_ = 0;
    // TODO: 记录每帧发送时间和大小（环形缓冲区），用于算 RTT 和带宽
};

}  // namespace tether::agent
