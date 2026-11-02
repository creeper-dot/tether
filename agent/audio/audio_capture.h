// 声音采集接口（各平台通用）：录"正在播放的声音"
//
// 实现：
//   linux    pulse_audio_capture（PulseAudio / PipeWire-Pulse 的 monitor source）
//   windows  wasapi_audio_capture（WASAPI loopback）
//   macos    coreaudio_capture（ScreenCaptureKit 的音频流，macOS 13+）
//
// 采集出来的 PCM 交给 opus_encoder（通用，libopus 各平台都有）。
//
// 通用要点：
//   - 格式固定：48000 Hz、2 声道、float32（Opus 支持）。设备格式不同时要重采样/转换
//   - 每次读一个 Opus 帧的量：48000 * 10ms = 480 个采样（每声道）
//   - 系统缓冲设小一些（10ms 左右），否则延迟高
//   - 在独立线程里阻塞读

#pragma once

#include <cstdint>
#include <vector>

import tether.util;

namespace tether::agent {

class AudioCapture {
public:
    virtual ~AudioCapture() = default;

    virtual Status start(std::uint32_t sample_rate = 48000, int channels = 2) = 0;

    // 阻塞读 frames 个采样帧（每帧 channels 个 float）
    virtual Status read(std::vector<float>& out, int frames) = 0;

    virtual void stop() = 0;
};

}  // namespace tether::agent
