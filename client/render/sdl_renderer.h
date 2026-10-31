// SDL2 窗口和画面显示
// 参考：
//   moonlight-qt/app/streaming/video/ffmpeg-renderers/sdlvid.cpp
//     第 416 行附近：SDL_CreateTexture
//     第 459 行附近：SDL_UpdateYUVTexture（YUV420P 用这个；BGRA 用 SDL_UpdateTexture）
//     第 582 行附近：SDL_RenderCopy + 呈现
//   moonlight-qt/app/streaming/video/ffmpeg-renderers/pacer/pacer.cpp
//     帧节奏控制：画面到达时间不均匀时怎么平滑显示（进阶，先不做）
//   moonlight-qt/app/streaming/streamutils.cpp   缩放和黑边的计算
//
// 要点：
//   - SDL 的窗口和渲染操作必须在主线程
//   - BGRA 对应 SDL_PIXELFORMAT_ARGB8888（小端机器上内存顺序是 B G R A）
//   - YUV420P 对应 SDL_PIXELFORMAT_IYUV
//   - 远端分辨率或格式变化时重建纹理
//   - 窗口缩放时保持宽高比、留黑边，算出 viewport 给 InputCapture 做坐标换算
//   - 关掉 vsync（SDL_RENDERER_PRESENTVSYNC）延迟更低，但可能有撕裂，做成选项
//
// 依赖：libsdl2-dev

#pragma once

#include "decode/decoder.h"
#include "input/input_capture.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace tether::client {

class SdlRenderer {
public:
    SdlRenderer();
    ~SdlRenderer();

    Status init(const char* title, int w, int h);
    void present(const DecodedFrame& frame);

    void on_window_resized();
    void set_fullscreen(bool on);
    void window_size(int& w, int& h) const;

    // 画面在窗口里实际显示的区域（给 InputCapture 换算坐标）
    Viewport viewport() const { return viewport_; }

    SDL_Window* window() const { return window_; }

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Texture* texture_ = nullptr;
    int tex_w_ = 0;
    int tex_h_ = 0;
    PixelFormat tex_format_ = PixelFormat::BGRA;
    Viewport viewport_;
};

}  // namespace tether::client
