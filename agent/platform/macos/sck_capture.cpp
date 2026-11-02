// macOS 截屏：Capturer 接口实现，底层调 sck_stream.mm 里的 ScreenCaptureKit
//
// ScreenCaptureKit 是"推"模式（有新画面就回调），我们的 Capturer::grab() 是"拉"模式，
// 中间用一个"只保留最新一帧"的缓冲衔接：
//   回调线程：拷贝画面到 latest_，new_frame_ = true，cv_.notify_one()
//   grab()：  cv_.wait_for(timeout_ms) 等 new_frame_
//             等到了 -> 交换 latest_ 和 out.pixels（swap 避免再拷一次）-> NewFrame
//             超时   -> NoChange
//             回调发现尺寸变了 -> 置 reinit_ 标志 -> 下次 grab 返回 Reinit
//   可以直接用 tether.util 里的 ThreadQueue（容量 1）代替手写 mutex + cv
//
// 权限：需要"屏幕录制"权限。CGPreflightScreenCaptureAccess() 检查，CGRequestScreenCaptureAccess() 申请。
//   授权后通常要重启 agent 才生效
//
// 参考：
//   Sunshine/src/platform/macos/display.mm
//     av_display_t::capture()（第 95 行附近）  推模式回调 + 信号量的衔接方式
//     display()（第 243 行附近）               按名字选显示器、CGGetActiveDisplayList
//   Sunshine/src/platform/macos/misc.mm
//     screen_recording_granted()、request_screen_recording()（第 83、171 行附近）  检查和申请权限

#include "platform/macos/backends.h"
#include "platform/macos/objc_bridge.h"

#include <condition_variable>
#include <mutex>

namespace tether::agent::macos_backend {

namespace {

class SckCapturer final : public Capturer {
public:
    explicit SckCapturer(const CaptureRegion& region) : region_(region) {}
    ~SckCapturer() override { objc::stream_stop(stream_); }

    Status init() {
        // TODO: 检查屏幕录制权限
        //       按 region_ 找显示器（CGGetActiveDisplayList + CGDisplayBounds，注意点和像素）
        //       objc::stream_start(opt, [this](const objc::VideoFrameView& f) { on_frame(f); }, nullptr, err)
        //       失败 return fail(Errc::System, err)
        return fail(Errc::Unsupported, "sck capture not implemented");
    }

    Result<GrabStatus> grab(RawFrame&, int) override {
        // TODO: 见文件开头
        return fail(Errc::Unsupported, "sck capture not implemented");
    }

    int width() const override { return width_; }
    int height() const override { return height_; }

private:
    void on_frame(const objc::VideoFrameView&) {
        // TODO: 在回调线程里拷贝到 latest_，通知 grab()
    }

    CaptureRegion region_;
    int width_ = 0;
    int height_ = 0;
    objc::ScreenStream* stream_ = nullptr;

    std::mutex mutex_;
    std::condition_variable cv_;
    RawFrame latest_;
    bool new_frame_ = false;
    bool reinit_ = false;
};

}  // namespace

Result<std::unique_ptr<Capturer>> create_sck_capturer(const CaptureRegion& region) {
    auto c = std::make_unique<SckCapturer>(region);
    if (auto st = c->init(); !st) {
        return std::unexpected(st.error());
    }
    return c;
}

}  // namespace tether::agent::macos_backend
