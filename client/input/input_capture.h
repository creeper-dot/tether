// 把 SDL 输入事件转成协议消息
// 参考：
//   moonlight-qt/app/streaming/input/input.cpp      总的事件分发；第 357 行附近：鼠标锁定（相对模式）
//   moonlight-qt/app/streaming/input/mouse.cpp      鼠标、滚轮、坐标换算
//   moonlight-qt/app/streaming/input/keyboard.cpp   键盘，以及"特殊组合键"（比如退出全屏）
//   tigervnc/vncviewer/Viewport.cxx                  键盘和鼠标处理，以及失去焦点时松开按键
//
// 要点：
//   - 窗口坐标 -> 远端坐标：画面可能缩放、有黑边，按渲染器给的显示区域换算
//   - 鼠标移动事件很密集，同一轮事件循环里只发最后一个位置
//   - 窗口失去焦点（SDL_WINDOWEVENT_FOCUS_LOST）时发 ReleaseAllKeys
//   - 保留一个本地快捷键（比如 Ctrl+Alt+Shift+Q）用来退出、切换全屏，不发给远端
//   - 鼠标锁定模式：SDL_SetRelativeMouseMode，发 MouseRelMove（玩游戏、用 3D 软件时需要）
//   - 滚轮用 SDL_MouseWheelEvent 的 preciseX/preciseY（SDL 2.0.18+）乘 120

#pragma once

#include <vector>

import tether.transport;

union SDL_Event;

namespace tether::client {

// 画面在窗口里实际显示的区域（缩放、黑边之后）
struct Viewport {
    int x = 0, y = 0, w = 0, h = 0;
};

enum class LocalAction { None, Quit, ToggleFullscreen, ToggleMouseLock };

struct OutgoingInput {
    Channel channel;
    Message msg;
};

class InputCapture {
public:
    void set_remote_size(int w, int h);
    void set_viewport(const Viewport& vp);

    // 处理一个 SDL 事件。需要发给 agent 的消息追加到 out；
    // 命中本地快捷键时返回对应动作
    LocalAction handle(const SDL_Event& ev, std::vector<OutgoingInput>& out);

    // 一轮事件处理完后调用：把合并后的鼠标位置发出去
    void flush(std::vector<OutgoingInput>& out);

private:
    int remote_w_ = 0, remote_h_ = 0;
    Viewport viewport_;
    bool mouse_moved_ = false;
    int pending_x_ = 0, pending_y_ = 0;
};

}  // namespace tether::client
