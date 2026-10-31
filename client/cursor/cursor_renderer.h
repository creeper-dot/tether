// 本地光标：用远端发来的光标形状替换 client 窗口里的系统光标
//
// 因为光标由 client 本地画，鼠标移动立刻有反应，不用等远端画面回来。
//
// 参考：
//   tigervnc/vncviewer/Viewport.cxx     第 198 行附近：setCursor()，把 RFB 光标变成本地光标
//   moonlight-qt/app/streaming/input/   搜 SDL_ShowCursor，看它什么时候隐藏本地光标
//   SDL2 文档：SDL_CreateRGBSurfaceFrom + SDL_CreateColorCursor
//
// 要点：
//   - 画面缩放时光标也要按比例缩放，否则大小不对
//   - 光标形状经常重复（箭头、I 形、手形），按内容哈希缓存 SDL_Cursor，避免反复创建
//   - 鼠标锁定模式下隐藏光标
//   - 远端自己移动了鼠标（CursorPos）时，用 SDL_WarpMouseInWindow 把本地光标挪过去

#pragma once

import tether.protocol;

struct SDL_Cursor;

namespace tether::client {

class CursorRenderer {
public:
    CursorRenderer();
    ~CursorRenderer();

    // 主线程调用
    void set_shape(const proto::CursorShape& shape, float scale);
    void set_visible(bool visible);

private:
    SDL_Cursor* current_ = nullptr;
    // TODO: 缓存
};

}  // namespace tether::client
