// Opus 音频编码
//
// 参考：
//   Sunshine/src/audio.cpp
//     第 121 行附近：opus_multistream_encoder_create（多声道用；立体声用 opus_encoder_create 就行）
//     第 131 行附近：设置码率
//   Opus 官方文档 https://opus-codec.org/docs/opus_api-1.5/  opus_encoder 一节
//
// 低延迟设置：
//   application = OPUS_APPLICATION_RESTRICTED_LOWDELAY
//   帧长 10ms（480 采样 @48kHz），也可以 5ms
//   码率：立体声 96~128 kbps 够用
//
// 依赖：libopus-dev

#pragma once

#include <cstdint>
#include <vector>

import tether.util;

namespace tether::agent {

class OpusEncoder {
public:
    OpusEncoder();
    ~OpusEncoder();

    Status init(std::uint32_t sample_rate, int channels, int bitrate_bps);

    // pcm 正好是一个 Opus 帧（比如 480 * channels 个 float），输出一个包
    Status encode(const std::vector<float>& pcm, std::vector<std::uint8_t>& out);

private:
    // TODO: ::OpusEncoder*（注意和类名同名，要写全局作用域 ::OpusEncoder）
};

}  // namespace tether::agent
