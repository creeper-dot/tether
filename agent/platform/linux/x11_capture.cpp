// X11 截屏
// 参考（从简单到复杂）：
//   tigervnc/unix/x0vncserver/Image.cxx          XImage / XShm 的创建和读取
//   tigervnc/unix/x0vncserver/XPixelBuffer.cxx   把 XImage 当成像素缓冲区用（86 行）
//   tigervnc/unix/x0vncserver/XDesktop.cxx       第 273 行附近：XDamageCreate，判断屏幕哪里变了
//   FreeRDP/server/shadow/X11/x11_shadow.c       另一份 XShm + XDamage 实现，C 写的，可以对照
//   Sunshine/src/platform/linux/x11grab.cpp      完整版，带光标合成、多显示器、CUDA
//
// 建议步骤：
//   1. XOpenDisplay + XGetImage 先跑通（慢，但最简单），可以先在 tools/grab_ppm.cpp 里练
//   2. 换成 XShm（XShmCreateImage + XShmGetImage），快很多
//   3. 加 XDamage：没有变化时返回 GrabStatus::NoChange
//   4. 订阅 RRScreenChangeNotify，分辨率变了返回 GrabStatus::Reinit
//
// 要点：
//   - 检查 XImage 的 bits_per_pixel、byte_order，不要假设一定是 32 位 BGRA
//   - XShm 要求 agent 和 X server 在同一台机器（远程桌面的 agent 本来就是）

#include "platform/linux/backends.h"

namespace tether::agent::linux_backend {

Result<std::unique_ptr<Capturer>> create_x11_capturer(const char*, const CaptureRegion&) {
    // TODO: 定义 class X11Capturer : public Capturer，打开 display 失败时
    //       return fail(Errc::System, "XOpenDisplay failed");
    return fail(Errc::Unsupported, "x11 capture not implemented");
}

}  // namespace tether::agent::linux_backend
