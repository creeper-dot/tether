// macOS 声音采集：用 ScreenCaptureKit 的音频输出录系统声音（macOS 13+）
//
// 和 sck_capture.cpp 一样是"推转拉"：
//   回调线程：把 interleaved float 追加到 ring buffer（std::deque<float> 或固定大小环形数组）
//   read(out, frames)：等到缓冲里够 frames * channels 个 float 再取出
//   缓冲积压太多（比如超过 100ms）就丢掉最旧的，防止延迟累积
//
// 另一种办法：Core Audio Process Tap（macOS 14.2+，AudioHardwareCreateProcessTap），
//   不需要屏幕录制权限，但 API 更底层。Sunshine 用的是这种，见下面的参考
//
// 参考：
//   obs-studio/plugins/mac-capture/mac-sck-audio-capture.m   ScreenCaptureKit 录声音
//   Sunshine/src/platform/macos/av_audio.mm
//     systemAudioIOProc()（第 124 行附近）     Process Tap 的 IO 回调
//     第 632-675 行附近                        CATapDescription + AudioHardwareCreateProcessTap
//     TPCircularBuffer                         它用的无锁环形缓冲，回调线程写、读线程读

#include "platform/macos/backends.h"
#include "platform/macos/objc_bridge.h"

#include <condition_variable>
#include <deque>
#include <mutex>

namespace tether::agent::macos_backend {

namespace {

class SckAudioCapture final : public AudioCapture {
public:
    ~SckAudioCapture() override { stop(); }

    Status start(std::uint32_t, int) override {
        // TODO: StreamOptions{.video = false, .audio = true, ...}，
        //       objc::stream_start(opt, nullptr, [this](const float* p, int n) { on_audio(p, n); }, err)
        //       注意：ScreenCaptureKit 不允许完全不要画面，video = false 时 .mm 里仍要配一个很小的
        //       画面（比如 2x2、1 fps）然后丢掉
        return fail(Errc::Unsupported, "sck audio capture not implemented");
    }

    Status read(std::vector<float>&, int) override {
        // TODO: 等缓冲够了再取
        return fail(Errc::Unsupported, "sck audio capture not implemented");
    }

    void stop() override {
        objc::stream_stop(stream_);
        stream_ = nullptr;
    }

private:
    void on_audio(const float*, int) {
        // TODO: 追加到 buffer_，超过上限丢旧数据，notify
    }

    objc::ScreenStream* stream_ = nullptr;
    int channels_ = 2;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::deque<float> buffer_;
};

}  // namespace

Result<std::unique_ptr<AudioCapture>> create_sck_audio_capture() {
    return std::make_unique<SckAudioCapture>();
}

}  // namespace tether::agent::macos_backend
