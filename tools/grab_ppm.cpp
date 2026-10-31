// 练习：X11 截屏存成 PPM 文件
//
// PPM 是最简单的图片格式，不需要任何库：
//   文件头一行文本 "P6\n<宽> <高>\n255\n"，后面直接是每个像素的 R G B 三个字节。
// 大部分看图软件都能打开（eog、gimp、feh）。
//
// 步骤：
//   1. XOpenDisplay(nullptr)，拿 DefaultRootWindow 和屏幕尺寸
//   2. XGetImage(root, 0, 0, w, h, AllPlanes, ZPixmap)
//   3. 逐像素 XGetPixel 或直接读 image->data（BGRA），写成 RGB
//   4. 跑通后换成 XShm 版本，用 Stopwatch 比较两者耗时
//
// 参考：tigervnc/unix/x0vncserver/Image.cxx
//
// 用法：DISPLAY=:99 ./grab_ppm out.ppm
// 写完后的代码搬进 agent/capture/x11_capture.cpp

#include <cstdio>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s out.ppm\n", argv[0]);
        return 2;
    }
    // TODO
    fprintf(stderr, "grab_ppm: not implemented yet\n");
    return 1;
}
