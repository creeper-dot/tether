// 测试 tether.io 的 fd 和 framing 分区
//
// 不需要 ssh，用 pipe() 造一对 fd：往 p[1] 写，从 p[0] 读。
// 注意：管道缓冲区一般只有 64KB，测大消息时写和读要放在两个线程里，
//       否则写端会一直阻塞等读端，程序卡死。

#include <cstdint>
#include <unistd.h>

#include "check.h"

import tether.io;

static void test_roundtrip() {
    int p[2];
    CHECK(pipe(p) == 0);

    tether::FrameWriter w(p[1]);
    tether::FrameReader r(p[0]);

    const std::uint8_t data[] = {1, 2, 3};
    CHECK(w.send(0x42, data));

    auto m = r.recv();
    CHECK(m.has_value());
    CHECK(m && m->type == 0x42);
    CHECK(m && m->payload.size() == 3);

    close(p[0]);
    close(p[1]);
}

// TODO: 自己补充
//   - 空 payload
//   - 连续发多条，按顺序收到
//   - 写端关闭后 recv() 返回 Errc::Closed
//   - 手工写一个 length = 0xFFFFFFFF 的头，recv() 返回 Errc::TooLarge
//   - 只写头不写完 payload 就关闭，recv() 返回 Errc::Truncated
//   - 读端关闭后 send() 返回 Errc::System 且 sys == EPIPE（先调 ignore_sigpipe()）
//   - 大消息（几 MB），用 std::jthread 一边写一边读
//   - 两个线程同时 send，收到的每条消息都完整（检验锁）

int main() {
    test_roundtrip();
    return test_result();
}
