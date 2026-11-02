// Windows 截屏：DXGI Desktop Duplication
//
// 原理：显卡驱动把"桌面合成后的画面"作为一张 GPU 纹理交给你，有变化才给新的一帧。
// 只能整屏截一个显示器（一个 IDXGIOutput），多显示器要各开一个，或者按 CaptureRegion 只开一个。
//
// 初始化（一次）：
//   1. CreateDXGIFactory1 -> EnumAdapters1 -> 每个 adapter EnumOutputs，
//      用 DXGI_OUTPUT_DESC.DesktopCoordinates 找到包含 CaptureRegion 的那个显示器
//      （笔记本双显卡时，显示器挂在哪块卡上就要用哪块卡建设备，否则 DuplicateOutput 失败）
//   2. D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, ...)
//   3. output->QueryInterface(IDXGIOutput1) -> DuplicateOutput(device, &dup)
//      （有 IDXGIOutput5 时用 DuplicateOutput1 并指定 DXGI_FORMAT_B8G8R8A8_UNORM，HDR 屏也能拿 8 位）
//   4. 建一张 staging 纹理：D3D11_USAGE_STAGING + D3D11_CPU_ACCESS_READ，尺寸和格式同桌面
//
// 每一帧 grab()：
//   1. dup->AcquireNextFrame(timeout_ms, &frame_info, &resource)
//        DXGI_ERROR_WAIT_TIMEOUT -> GrabStatus::NoChange
//        DXGI_ERROR_ACCESS_LOST  -> 分辨率变了、切到锁屏/UAC 桌面、全屏独占游戏
//                                  -> GrabStatus::Reinit（调用方重建；重建前要 sync_input_desktop()）
//   2. frame_info.LastPresentTime == 0 表示只有鼠标动了、画面没变 -> NoChange
//      （光标信息走 win_cursor_monitor，这里不处理，DXGI 默认也不把光标画进画面）
//   3. resource -> QueryInterface(ID3D11Texture2D)
//      检查纹理尺寸/格式，和初始化时不一样 -> Reinit
//   4. context->CopyResource(staging, texture)
//   5. context->Map(staging, 0, D3D11_MAP_READ, ...) -> 按 RowPitch 拷进 RawFrame
//      （RowPitch 就是 stride，可能比 width*4 大）-> Unmap
//   6. dup->ReleaseFrame()
//      必须在下一次 AcquireNextFrame 之前调用；不释放的话系统会一直等你，桌面会卡
//
// 以后优化：
//   - 不拷到内存，直接把 GPU 纹理交给硬件编码器（NVENC / AMF / QSV 的 D3D11 输入），省掉 Map
//   - Windows.Graphics.Capture（Win10 1903+）：能截单个窗口，没有 ACCESS_LOST 的问题，
//     但光标、黄边框要处理。Sunshine 两种都有，DXGI 是默认
//
// 参考：
//   Sunshine/src/platform/windows/display_base.cpp
//     duplication_t::init()（第 64 行附近）       DuplicateOutput / DuplicateOutput1
//     duplication_t::next_frame()（第 150 行附近）AcquireNextFrame 和各种错误码的处理
//     display_base_t::init()（第 463 行附近）     找 adapter/output、建设备
//   Sunshine/src/platform/windows/display_ram.cpp
//     display_ddup_ram_t::snapshot()              staging 纹理 + Map 拷到内存（我们就是这种做法）
//   Sunshine/src/platform/windows/display_wgc.cpp  Windows.Graphics.Capture 版本
//   learn.microsoft.com "Desktop Duplication API"，以及微软的 DesktopDuplication 示例程序
//
// 依赖：d3d11 dxgi（系统自带）

#include "platform/windows/backends.h"
#include "platform/windows/win_util.h"

// #include <d3d11.h>
// #include <dxgi1_2.h>
// #include <wrl/client.h>    // Microsoft::WRL::ComPtr，COM 指针自动 Release

namespace tether::agent::windows_backend {

namespace {

class DxgiCapturer final : public Capturer {
public:
    explicit DxgiCapturer(const CaptureRegion& region) : region_(region) {}

    // 构造函数不能返回错误，初始化放在单独的函数里，由 create_dxgi_capturer 调用
    Status init() {
        // TODO: sync_input_desktop() -> 找 adapter/output -> D3D11CreateDevice -> DuplicateOutput
        //       -> 建 staging 纹理，记下 width_/height_
        return fail(Errc::Unsupported, "dxgi capture not implemented");
    }

    Result<GrabStatus> grab(RawFrame&, int) override {
        // TODO: 见文件开头"每一帧 grab()"
        return fail(Errc::Unsupported, "dxgi capture not implemented");
    }

    int width() const override { return width_; }
    int height() const override { return height_; }

private:
    CaptureRegion region_;
    int width_ = 0;
    int height_ = 0;
    // TODO: ComPtr<ID3D11Device> device_、ComPtr<ID3D11DeviceContext> context_、
    //       ComPtr<IDXGIOutputDuplication> dup_、ComPtr<ID3D11Texture2D> staging_
};

}  // namespace

Result<std::unique_ptr<Capturer>> create_dxgi_capturer(const CaptureRegion& region) {
    auto c = std::make_unique<DxgiCapturer>(region);
    if (auto st = c->init(); !st) {
        return std::unexpected(st.error());
    }
    return c;
}

}  // namespace tether::agent::windows_backend
