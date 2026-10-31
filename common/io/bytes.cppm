// tether.io:bytes  字节序列化
// 参考：tigervnc/common/rdr/InStream.h、OutStream.h、MemInStream.h、MemOutStream.h
//
// 要点：
//   - 协议里的整数统一大端（高字节在前）。C++23 有 std::byteswap 和 std::endian::native，
//     也可以直接移位逐字节写，两种都和机器字节序无关
//   - Reader 每次读之前检查剩余长度，不够就返回 Errc::Malformed。对端数据不可信，绝不能读越界
//   - 读字符串、字节数组时先检查长度上限再分配内存，防止对端发个 0xFFFFFFFF 让你分配 4GB
//
// 错误处理：
//   Reader 的每个读函数都返回 Result<T>。一条消息有很多字段，每个都写 if (!x) return ...
//   会很啰嗦，所以 Reader 采用"粘性错误"的写法：
//     - 读失败时记下第一个错误，之后的读都直接返回 0 / 空
//     - 解析完一条消息后只检查一次 r.finish()
//   这样 decode 函数可以写得像普通顺序代码：
//
//     ByteReader r(msg.payload);
//     MouseMove m;
//     m.x = r.i32();
//     m.y = r.i32();
//     if (auto st = r.finish(); !st) return std::unexpected(st.error());
//     return m;
//
// 和 TigerVNC 的区别：我们一条消息的 payload 收完整再解析，
// 所以不需要 TigerVNC 那套 overrun()/restorePoint 机制，简单很多。
//
// 测试：tests/test_bytes.cpp

module;

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

export module tether.io:bytes;

import tether.util;

export namespace tether {

// 往 vector 末尾追加
class ByteWriter {
public:
    void u8(std::uint8_t v) { (void)v; /* TODO */ }
    void u16(std::uint16_t v) { (void)v; /* TODO */ }
    void u32(std::uint32_t v) { (void)v; /* TODO */ }
    void u64(std::uint64_t v) { (void)v; /* TODO */ }
    void i16(std::int16_t v) { (void)v; /* TODO */ }
    void i32(std::int32_t v) { (void)v; /* TODO */ }
    void boolean(bool v) { u8(v ? 1 : 0); }
    void bytes(std::span<const std::uint8_t> data) { (void)data; /* TODO */ }
    void blob(std::span<const std::uint8_t> data) { (void)data; /* TODO: 先写 u32 长度，再写内容 */ }
    void str(std::string_view s) { (void)s; /* TODO: 先写 u32 长度，再写内容 */ }

    const std::vector<std::uint8_t>& data() const { return buf_; }
    std::vector<std::uint8_t> take() { return std::move(buf_); }
    std::size_t size() const { return buf_.size(); }

private:
    std::vector<std::uint8_t> buf_;
};

// 从一块内存里按顺序读，粘性错误（见文件开头说明）
class ByteReader {
public:
    explicit ByteReader(std::span<const std::uint8_t> data) : data_(data) {}

    std::size_t remaining() const { return data_.size() - pos_; }
    bool ok() const { return !error_.has_value(); }

    std::uint8_t u8() { /* TODO */ return 0; }
    std::uint16_t u16() { /* TODO */ return 0; }
    std::uint32_t u32() { /* TODO */ return 0; }
    std::uint64_t u64() { /* TODO */ return 0; }
    std::int16_t i16() { /* TODO */ return 0; }
    std::int32_t i32() { /* TODO */ return 0; }
    bool boolean() { /* TODO: 只接受 0 和 1 */ return false; }

    // 读 u32 长度 + 内容；长度超过 max_len 记为 Errc::TooLarge
    std::vector<std::uint8_t> blob(std::size_t max_len) { (void)max_len; /* TODO */ return {}; }
    std::string str(std::size_t max_len = 1 << 20) { (void)max_len; /* TODO */ return {}; }

    // 解析结束时调用：之前有错误就返回那个错误；没错误但还有剩余字节 -> Errc::Malformed
    Status finish() const { /* TODO */ return {}; }

private:
    std::span<const std::uint8_t> data_;
    std::size_t pos_ = 0;
    std::optional<Error> error_;
};

}  // namespace tether
