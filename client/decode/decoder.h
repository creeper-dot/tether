// 解码器接口：VideoFrame -> 可以显示的图像
//
// 实现：
//   raw_decoder     原样拷贝，MVP
//   ffmpeg_decoder  H.264/HEVC，软件解码或 VAAPI 硬件解码
//
// 参考：
//   moonlight-qt/app/streaming/video/decoder.h    解码器接口设计
//   moonlight-qt/app/streaming/video/ffmpeg.cpp   完整的 FFmpeg 解码，包括硬解选择和出错恢复

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

import tether.protocol;

namespace tether::client {

enum class PixelFormat {
    BGRA,      // raw_decoder 输出
    YUV420P,   // ffmpeg 软解输出，SDL 可以直接显示（SDL_PIXELFORMAT_IYUV）
};

struct DecodedFrame {
    std::uint64_t frame_id = 0;
    std::uint64_t capture_time_us = 0;
    int width = 0;
    int height = 0;
    PixelFormat format = PixelFormat::BGRA;
    // BGRA 只用 planes[0]；YUV420P 用 0/1/2 三个平面
    std::vector<std::uint8_t> planes[3];
    int strides[3] = {0, 0, 0};
};

class Decoder {
public:
    virtual ~Decoder() = default;

    // 失败（数据损坏）时调用方应该发 RequestKeyframe
    virtual Status decode(const proto::VideoFrame& in, DecodedFrame& out) = 0;
};

std::unique_ptr<Decoder> create_raw_decoder();

#ifdef TETHER_HAVE_FFMPEG
Result<std::unique_ptr<Decoder>> create_ffmpeg_decoder(proto::Codec codec, bool prefer_hardware);
#endif

}  // namespace tether::client
