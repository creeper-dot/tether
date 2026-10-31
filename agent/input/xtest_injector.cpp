// XTest 输入注入
// 参考：
//   tigervnc/unix/x0vncserver/XDesktop.cxx
//     第 158 行附近：XTestQueryExtension 检查扩展是否存在，XTestGrabControl
//     第 367-377 行：XTestFakeMotionEvent / XTestFakeButtonEvent
//     第 616 行附近：XTestFakeKeyEvent，以及前面 keysym -> keycode 的转换
//   FreeRDP/server/shadow/X11/x11_shadow.c  搜 XTestFake，另一份实现
//
// 要点：
//   - 每次注入后 XFlush(dpy)，否则事件会留在 Xlib 缓冲区里不发出去
//   - 滚轮在 X11 里是按钮 4/5（竖）和 6/7（横），按下再抬起算一格；
//     细粒度滚动要自己累加到 120 再发一次
//   - 相对移动：XTestFakeRelativeMotionEvent
//   - 记录当前按下的键，release_all() 时逐个抬起
//
// 依赖：libxtst-dev

#include "input/injector.h"

namespace tether::agent {

Result<std::unique_ptr<Injector>> create_xtest_injector(const char*) {
    // TODO: 定义 class XTestInjector : public Injector
    return fail(Errc::Unsupported, "create_xtest_injector not implemented");
}

}  // namespace tether::agent
