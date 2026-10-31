// 声音采集（PulseAudio / PipeWire-Pulse）
//
// 原理：每个声卡输出（sink）都有一个对应的 monitor source，录它就能拿到"正在播放的声音"。
// 现在大部分发行版用 PipeWire，但它兼容 PulseAudio 的 API，所以用 libpulse 两边都能跑。
//
// 参考：
//   Sunshine/src/platform/linux/audio.cpp
//     第 163 行附近：pa_simple_new 创建录音流
//     第 594 行附近：get_monitor_name()，找默认输出对应的 monitor
//   tigervnc/vncviewer/AudioOutputPulse.cxx  播放端，但可以看 pa_simple 的用法
//   PulseAudio 文档：pa_simple API（最简单的同步接口，够用）
//
// 要点：
//   - 格式固定：48000 Hz、2 声道、float32 或 s16（Opus 支持这些）
//   - 每次读一个 Opus 帧的量：48000 * 10ms = 480 个采样（每声道）
//   - 设 pa_buffer_attr 的 fragsize 小一些（10ms），否则 PulseAudio 默认缓冲很大，延迟高
//   - 在独立线程里阻塞读
//
// 依赖：libpulse-dev

#pragma once

#include <cstdint>
#include <vector>

import tether.util;

namespace tether::agent {

class AudioCapture {
public:
    AudioCapture();
    ~AudioCapture();

    Status start(std::uint32_t sample_rate = 48000, int channels = 2);

    // 阻塞读 frames 个采样帧（每帧 channels 个 float）
    Status read(std::vector<float>& out, int frames);

    void stop();

private:
    // TODO: pa_simple*
};

}  // namespace tether::agent
