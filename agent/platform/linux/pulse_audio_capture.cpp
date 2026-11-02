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
//   - 设 pa_buffer_attr 的 fragsize 小一些（10ms），否则 PulseAudio 默认缓冲很大，延迟高
//
// 依赖：libpulse-dev

#include "platform/linux/backends.h"

// #include <pulse/simple.h>
// #include <pulse/error.h>

namespace tether::agent::linux_backend {

namespace {

class PulseAudioCapture final : public AudioCapture {
public:
    Status start(std::uint32_t, int) override {
        // TODO: 找默认 sink 的 monitor -> pa_simple_new(PA_STREAM_RECORD)
        return fail(Errc::Unsupported);
    }

    Status read(std::vector<float>&, int) override {
        // TODO: pa_simple_read
        return fail(Errc::Unsupported);
    }

    void stop() override {
        // TODO: pa_simple_free
    }

private:
    // TODO: pa_simple*
};

}  // namespace

Result<std::unique_ptr<AudioCapture>> create_pulse_audio_capture() {
    return std::make_unique<PulseAudioCapture>();
}

}  // namespace tether::agent::linux_backend
