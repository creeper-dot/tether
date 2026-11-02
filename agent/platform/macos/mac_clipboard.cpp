// macOS 剪贴板：Clipboard 接口实现，底层调 pasteboard.mm
//
// 变化检测：比较 objc::pasteboard_change_count()，变了才读
// 防回环：set() 之后记下 change_count，poll() 时等于它就跳过
//
// 参考：见 pasteboard.mm

#include "platform/macos/backends.h"
#include "platform/macos/objc_bridge.h"

namespace tether::agent::macos_backend {

namespace {

class MacClipboard final : public Clipboard {
public:
    bool poll(std::string&) override {
        // TODO
        return false;
    }

    void set(const std::string&) override {
        // TODO
    }

private:
    long last_count_ = 0;
    long self_set_count_ = -1;
};

}  // namespace

Result<std::unique_ptr<Clipboard>> create_mac_clipboard() {
    return std::make_unique<MacClipboard>();
}

}  // namespace tether::agent::macos_backend
