// Objective-C API 的 C++ 包装（macOS）
//
// 为什么要这一层：
//   ScreenCaptureKit、NSPasteboard、NSCursor 是 Objective-C API，只能在 .mm（Objective-C++）里调。
//   但 CMake 只对 .cpp 做 C++ 模块扫描，.mm 里不能写 import tether.util;，
//   也就不能 include capturer.h 这些接口头文件。
//   所以分两层：
//     .mm   只管调 Objective-C，对外暴露这个头文件里的纯 C++ 函数（只用标准库类型）
//     .cpp  实现 Capturer / Clipboard 等接口，调用这里的函数，把 error 字符串转成 Error
//
// 这个头文件不能 include 任何 import 了模块的头文件，也不能出现 Objective-C 语法。
//
// 内存管理：.mm 文件用 ARC 编译（-fobjc-arc，见 CMakeLists.txt），Objective-C 对象不用手动 release。
// 跨到 C++ 这边的 Objective-C 对象用不透明指针（ScreenStream*）持有。

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace tether::agent::macos_backend::objc {

// ============================ ScreenCaptureKit（sck_stream.mm）============================

struct VideoFrameView {
    const std::uint8_t* data = nullptr;   // BGRA，回调返回后失效，要自己拷贝
    int width = 0;                        // 像素（Retina 屏上是点数的 2 倍）
    int height = 0;
    int stride = 0;                       // CVPixelBufferGetBytesPerRow
    std::uint64_t timestamp_us = 0;
};

struct StreamOptions {
    std::uint32_t display_id = 0;         // CGDirectDisplayID；0 表示主屏 CGMainDisplayID()
    int fps = 60;
    bool video = true;
    bool audio = false;                   // macOS 13+
    int sample_rate = 48000;
    int channels = 2;
};

// 回调在 ScreenCaptureKit 的 dispatch queue 线程上调用，不是调用 stream_start 的线程
using VideoCallback = std::function<void(const VideoFrameView&)>;
// interleaved float32，frames 个采样帧（每帧 channels 个 float）
using AudioCallback = std::function<void(const float* interleaved, int frames)>;

struct ScreenStream;   // 不透明，内部持有 SCStream 和 delegate 对象

// 阻塞直到流启动成功或失败。失败返回 nullptr，原因写进 error
ScreenStream* stream_start(const StreamOptions& opt, VideoCallback on_video, AudioCallback on_audio,
                           std::string& error);
void stream_stop(ScreenStream* s);   // 可以传 nullptr

// ============================ 剪贴板（pasteboard.mm）============================

// NSPasteboard.changeCount，内容每变一次加一
long pasteboard_change_count();
// 没有文本返回 false
bool pasteboard_get_text(std::string& utf8);
void pasteboard_set_text(const std::string& utf8);

// ============================ 光标（ns_cursor.mm）============================

struct CursorImage {
    int width = 0, height = 0;            // 像素
    int hotspot_x = 0, hotspot_y = 0;     // 像素
    std::vector<std::uint8_t> rgba;       // 预乘 alpha
};

// 取当前系统光标的图像。失败返回 false
bool current_cursor_image(CursorImage& out);
// 用来判断光标形状有没有变：同一个光标返回同一个值（NSCursor 对象地址，或图像数据的哈希）
std::uintptr_t current_cursor_id();

}  // namespace tether::agent::macos_backend::objc
