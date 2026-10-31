// 测试 agent/flow/flow_control.h

#include "check.h"
#include "flow/flow_control.h"

static void test_limit() {
    tether::agent::FlowControl fc(2);
    CHECK(fc.can_send());
    fc.on_sent(1, 1000);
    fc.on_sent(2, 1000);
    CHECK(!fc.can_send());   // 在途 2 帧，到上限
    fc.on_ack(1);
    CHECK(fc.can_send());
}

// TODO: 自己补充
//   - 重复 ack 同一帧，不会让在途数变成负数
//   - 乱序 ack（先 ack 2 再 ack 1），状态正确
//   - ack 一个从没发过的帧号（对端出错），不崩溃
//   - rtt_us()：on_sent 和 on_ack 之间 sleep 一下，结果大致对

int main() {
    test_limit();
    return test_result();
}
