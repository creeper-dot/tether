// 练习：XTest 注入鼠标和键盘
//
// 目标：运行后鼠标自己移到屏幕中间、点一下左键，然后打出 "hello"
//
// 用 Xvfb 测试，避免误操作自己的桌面：
//   Xvfb :99 -screen 0 1280x720x24 &
//   DISPLAY=:99 xterm &
//   DISPLAY=:99 ./xtest_demo
//   DISPLAY=:99 ./grab_ppm out.ppm      看看 xterm 里有没有出现 hello
//
// 参考：tigervnc/unix/x0vncserver/XDesktop.cxx
//   第 158 行附近：XTestQueryExtension
//   第 367-377 行：鼠标
//   第 616 行附近：键盘
//
// 写完后的代码搬进 agent/input/xtest_injector.cpp

#include <cstdio>

int main() {
    // TODO: XOpenDisplay -> XTestQueryExtension -> XTestFakeMotionEvent -> XTestFakeButtonEvent
    //       -> XKeysymToKeycode + XTestFakeKeyEvent -> XFlush -> XCloseDisplay
    fprintf(stderr, "xtest_demo: not implemented yet\n");
    return 1;
}
