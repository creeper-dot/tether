// H.264 / HEVC 编码（FFmpeg libavcodec）
//
// 参考（按阅读顺序）：
//   FFmpeg 源码 doc/examples/encode_video.c       最小的编码示例，先照着它写 tools/h264_file.cpp
//   FFmpeg 源码 doc/examples/vaapi_encode.c       VAAPI 硬件编码示例
//   Sunshine/src/video.cpp
//     第 784 行附近：NVENC 参数（forced-idr、zerolatency）
//     第 881 行附近：VAAPI/QSV 参数
//     搜 "intra-refresh"：用 intra refresh 代替周期性关键帧，码率更平稳
//   Sunshine/src/platform/linux/vaapi.cpp          VAAPI 硬件帧上传
//
// 低延迟设置（libx264）：
//   preset=ultrafast 或 superfast，tune=zerolatency
//   max_b_frames = 0（B 帧需要等后面的帧，增加延迟）
//   gop_size 设很大（比如 fps * 60），关键帧只在 request_keyframe() 时出
//   码率控制：CBR 或 VBV（rc_max_rate + rc_buffer_size ≈ 1 帧的量）
//   每输入一帧必须立刻输出一帧（zerolatency 保证这点）
//
// 颜色转换：
//   截屏是 BGRA，编码器要 YUV420P（或 NV12），用 libswscale 的 sws_scale 转换。
//   注意 colorspace（BT.709）和 range（limited），解码端要一致，否则颜色发灰或过饱和。
//   参考 Sunshine/src/video_colorspace.cpp
//
// 硬件编码（第二阶段）：
//   优先级 NVENC（h264_nvenc）> VAAPI（h264_vaapi）> 软件 libx264
//   创建失败时自动退回下一个

#include "encode/encoder.h"

// extern "C" {
// #include <libavcodec/avcodec.h>
// #include <libavutil/opt.h>
// #include <libswscale/swscale.h>
// }

namespace tether::agent {

Result<std::unique_ptr<Encoder>> create_ffmpeg_encoder(const EncoderConfig&) {
    // TODO: 定义 class FfmpegEncoder : public Encoder
    return fail(Errc::Unsupported, "ffmpeg encoder not implemented");
}

}  // namespace tether::agent
