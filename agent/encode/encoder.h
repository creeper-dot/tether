// 编码器接口：RawFrame -> 可以发出去的字节
//
// 实现：
//   raw_encoder     不压缩，MVP 用来验证链路
//   ffmpeg_encoder  H.264/HEVC，软件 libx264 或硬件 VAAPI/NVENC
//
// 参考：
//   Sunshine/src/video.h            encoder_t、config_t：编码器能力、参数怎么组织
//   FreeRDP/server/shadow/shadow_encoder.c   RDP 服务端怎么管理多种编码器

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "capture/capturer.h"

import tether.protocol;

namespace tether::agent {

struct EncodedFrame {
    proto::Codec codec = proto::Codec::Raw;
    bool keyframe = true;
    std::vector<std::uint8_t> data;
};

struct EncoderConfig {
    proto::Codec codec = proto::Codec::H264;
    int width = 0;
    int height = 0;
    int fps = 60;
    int bitrate_kbps = 8000;
    bool prefer_hardware = true;    // 优先 NVENC / VAAPI，失败退回 libx264
};

class Encoder {
public:
    virtual ~Encoder() = default;

    // 一帧进一帧出（低延迟模式下编码器不能攒帧）
    virtual Status encode(const RawFrame& in, EncodedFrame& out) = 0;

    // 下一帧强制输出关键帧（client 刚连上、解码出错之后需要）
    virtual void request_keyframe() = 0;

    // 运行中调码率（网络变差时降码率）。不支持的实现可以忽略
    virtual void set_bitrate(int kbps) { (void)kbps; }
};

std::unique_ptr<Encoder> create_raw_encoder();

#ifdef TETHER_HAVE_FFMPEG
// 硬件编码器创建失败时内部自动退回软件编码；全部失败才返回错误
Result<std::unique_ptr<Encoder>> create_ffmpeg_encoder(const EncoderConfig& cfg);
#endif

}  // namespace tether::agent
