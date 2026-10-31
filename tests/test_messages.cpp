// 测试 tether.protocol
//
// 每种消息：构造一个 struct -> encode -> decode -> 每个字段都相等

#include "check.h"

import tether.protocol;

using namespace tether::proto;

static void test_mouse_move() {
    MouseMove in;
    in.x = -5;
    in.y = 1080;

    tether::Message msg = encode(in);
    CHECK(msg.type == kMouseMove);

    auto out = decode_mouse_move(msg);
    CHECK(out.has_value());
    CHECK(out && out->x == -5);
    CHECK(out && out->y == 1080);
}

// TODO: 自己补充
//   - 每种 [MVP] 消息的往返
//   - VideoFrame 带一段数据的往返
//   - 拿错 type 的消息去 decode（比如把 Key 交给 decode_mouse_move）返回 Errc::Malformed
//   - payload 截短一个字节，decode 返回错误
//   - payload 多一个字节，decode 返回错误
//   - CursorShape 的 rgba 长度和 width*height*4 不符，decode 返回错误

int main() {
    test_mouse_move();
    return test_result();
}
