// 当前系统光标的图像
//
//   NSCursor *c = [NSCursor currentSystemCursor];     整个系统当前的光标（不是本进程的）
//     注意：返回的对象每次可能不同，不能直接拿地址判断"形状变了没有"，
//     可以对图像数据算个哈希，或者比较 hotSpot + 图像尺寸 + 前几行像素
//   c.hotSpot                                         热点，单位是点
//   c.image（NSImage）-> CGImageForProposedRect:context:hints: 得到 CGImageRef
//   用 CGBitmapContextCreate 建一个 RGBA 预乘（kCGImageAlphaPremultipliedLast）的位图，
//   CGContextDrawImage 画进去，就得到协议要的 RGBA 预乘数据
//   Retina：CGImage 的像素尺寸一般是 image.size（点）的 2 倍，热点也要乘同样的比例
//
// 限制：
//   currentSystemCursor 有时拿不到应用自定义的光标（返回 nil 或者箭头），这是 API 本身的限制。
//   需要更准确的话可以让 ScreenCaptureKit 把光标画进画面（showsCursor = YES），放弃本地画光标
//
// 参考：
//   Apple 文档 "NSCursor"、"CGBitmapContextCreate"

#include "platform/macos/objc_bridge.h"

// #import <AppKit/AppKit.h>

namespace tether::agent::macos_backend::objc {

bool current_cursor_image(CursorImage&) {
    // TODO
    return false;
}

std::uintptr_t current_cursor_id() {
    // TODO
    return 0;
}

}  // namespace tether::agent::macos_backend::objc
