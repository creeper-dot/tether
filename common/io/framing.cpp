// tether.io:framing 的实现
module;

#include <cstdint>
#include <span>

module tether.io;

namespace tether {

Status FrameWriter::send(std::uint8_t, std::span<const std::uint8_t>) {
    // TODO: 检查长度，拼 5 字节头，加锁后 write_all 头和 payload
    return {};
}

Result<Message> FrameReader::recv() {
    // TODO: read_exact 头 -> 解析 type/length -> 检查 length -> read_exact payload
    //       payload 读取返回 Closed 时要改成 Truncated
    return Message{};
}

}  // namespace tether
