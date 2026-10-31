// 练习：把连续截屏编码成 H.264 文件
//
// 目标：截 5 秒屏幕，编码成 out.h264，然后 ffplay out.h264 能正常播放
//
// 步骤：
//   1. 先照着 FFmpeg 源码 doc/examples/encode_video.c 写一遍，用它生成的测试画面
//   2. 把测试画面换成 X11 截屏（复用 grab_ppm 的代码）
//   3. 加 sws_scale 做 BGRA -> YUV420P
//   4. 设低延迟参数（见 agent/encode/ffmpeg_encoder.cpp 开头的注释），对比文件大小和画质
//   5. 打印每帧编码耗时，1080p 软件编码应该在 10ms 以内
//
// 写完后的代码搬进 agent/encode/ffmpeg_encoder.cpp

#include <cstdio>

int main() {
    // TODO
    fprintf(stderr, "h264_file: not implemented yet\n");
    return 1;
}
