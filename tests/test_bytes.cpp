// 测试 tether.io 的 bytes 分区

#include <cstdint>

#include "check.h"

import tether.io;

static void test_u16_big_endian() {
    tether::ByteWriter w;
    w.u16(0x1234);
    CHECK(w.size() == 2);
    CHECK(w.data()[0] == 0x12);  // 大端：高字节在前
    CHECK(w.data()[1] == 0x34);
}

static void test_short_read_is_error() {
    const std::uint8_t data[] = {0x00, 0x01};
    tether::ByteReader r(data);
    (void)r.u32();               // 只有 2 字节，读 4 字节应该失败
    auto st = r.finish();
    CHECK(!st);
    CHECK(!st && st.error().code == tether::Errc::Malformed);
}

// TODO: 自己补充
//   - u8/u32/u64 写进去再读出来，值不变
//   - i32(-1) 往返后还是 -1
//   - u32 最高位为 1 的值（比如 0xFFFFFFFF）往返正确
//   - str / blob 往返，包括空字符串
//   - 粘性错误：第一次读失败后，后面的读都返回 0，finish() 报的是第一个错误
//   - str() 长度字段超过 max_len 时 finish() 返回 Errc::TooLarge
//   - 有剩余字节时 finish() 返回 Errc::Malformed

int main() {
    test_u16_big_endian();
    test_short_read_is_error();
    return test_result();
}
