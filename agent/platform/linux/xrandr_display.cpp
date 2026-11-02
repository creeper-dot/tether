// X11 显示器布局和改分辨率（XRandR）
//
// 参考：
//   tigervnc/unix/common/randr.cxx
//     computeScreenLayout()：读取当前显示器布局
//     setScreenLayout()：按 client 要求改分辨率
//   tigervnc/unix/x0vncserver/RandrGlue.c   XRandR 调用的封装，第 131、374 行
//
// 要点：
//   - 真实显示器只能切到 XRRGetScreenResources 列出来的模式
//   - Xvfb 和 Xvnc 可以设任意分辨率，需要先 XRRCreateMode 再加到 output 上
//
// 依赖：libxrandr-dev

#include "platform/linux/backends.h"

namespace tether::agent::linux_backend {

namespace {

class XrandrDisplay final : public DisplayManager {
public:
    explicit XrandrDisplay(const char*) {
        // TODO: XOpenDisplay，检查 XRandR 扩展版本 >= 1.2
    }

    proto::DisplayLayout layout() override {
        // TODO: XRRGetScreenResources + XRRGetCrtcInfo，填 screens
        return {};
    }

    Status resize(std::uint32_t, int, int) override {
        // TODO
        return fail(Errc::Unsupported);
    }

    void restore() override {
        // TODO
    }

    bool poll_changed() override {
        // TODO: XRRSelectInput 订阅 RRScreenChangeNotifyMask，这里检查事件
        return false;
    }

private:
    // TODO: Display*、原始配置
};

}  // namespace

Result<std::unique_ptr<DisplayManager>> create_xrandr_display(const char* display) {
    return std::make_unique<XrandrDisplay>(display);
}

}  // namespace tether::agent::linux_backend
