// 练习：SDL 窗口显示图片 + 打印输入事件
//
// 第一步：打开 grab_ppm 生成的 PPM 文件，显示在窗口里
// 第二步：把窗口里的鼠标、键盘事件打印到终端（SDL_PollEvent）
// 第三步（本地镜像）：每帧在同一进程里截屏并显示，看到"自己的屏幕在窗口里实时显示"
//
// 参考：
//   moonlight-qt/app/streaming/video/ffmpeg-renderers/sdlvid.cpp
//     第 416 行附近：SDL_CreateTexture
//     第 582 行附近：SDL_RenderCopy
//   moonlight-qt/app/streaming/input/input.cpp    事件循环的写法
//   SDL2 官方 wiki：SDL_CreateWindow、SDL_UpdateTexture、SDL_PollEvent
//
// 用法：./sdl_view out.ppm
// 写完后的代码搬进 client/render/ 和 client/input/

#include <cstdio>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s image.ppm\n", argv[0]);
        return 2;
    }
    // TODO
    fprintf(stderr, "sdl_view: not implemented yet\n");
    return 1;
}
