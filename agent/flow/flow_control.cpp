#include "flow/flow_control.h"

namespace tether::agent {

bool FlowControl::can_send() const {
    // TODO: last_sent_ - last_acked_ < max_in_flight_
    return true;
}

void FlowControl::on_sent(std::uint64_t, std::size_t) {
    // TODO: 记录 last_sent_ 和发送时间
}

void FlowControl::on_ack(std::uint64_t) {
    // TODO: ack 可能乱序或重复，只在 frame_id 更大时更新；顺便算 RTT
}

std::uint64_t FlowControl::rtt_us() const {
    // TODO
    return 0;
}

int FlowControl::suggested_bitrate_kbps() const {
    // TODO（第二步）
    return 0;
}

}  // namespace tether::agent
