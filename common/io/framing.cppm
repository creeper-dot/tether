// tether.io:framing  消息分帧
// 参考：tigervnc/common/rfb/CMsgReader.cxx（看它怎么一条条读消息，
//       但 RFB 没有统一长度头，我们的格式更简单）
//
// 管道/TCP 是连续字节流，没有消息边界：发两次可能一次收到，也可能分三次收齐。
// 所以每条消息前加一个头：
//
//   +---------+-------------+------------------+
//   | type u8 | length u32  | payload (length) |
//   +---------+-------------+------------------+
//
// 要点：
//   - 读：先 read_exact 5 字节头，检查 length <= kMaxPayload，再读 payload
//         头读到了但 payload 没读完就 EOF -> Errc::Truncated（不是 Closed）
//   - 写：多线程可能同时发（视频线程、控制线程），头和 payload 要在同一把锁里写完，
//         否则两条消息的字节会交错
//
// 测试：tests/test_framing.cpp

module;

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <span>
#include <vector>

export module tether.io:framing;

import tether.util;
import :fd;

export namespace tether {

struct Message {
    std::uint8_t type = 0;
    std::vector<std::uint8_t> payload;
};

inline constexpr std::size_t kHeaderSize = 5;
inline constexpr std::uint32_t kMaxPayload = 64u * 1024 * 1024;  // 4K 未压缩 BGRA 约 33MB，留余量

class FrameWriter {
public:
    explicit FrameWriter(NativeHandle h) : fd_(h) {}

    // 线程安全
    Status send(std::uint8_t type, std::span<const std::uint8_t> payload);
    Status send(const Message& m) { return send(m.type, m.payload); }

private:
    NativeHandle fd_;
    std::mutex mutex_;
};

class FrameReader {
public:
    explicit FrameReader(NativeHandle h) : fd_(h) {}

    // 阻塞读一条完整消息。只能由一个线程调用。
    Result<Message> recv();

private:
    NativeHandle fd_;
};

}  // namespace tether
