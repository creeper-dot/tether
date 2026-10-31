#include "display/display_manager.h"

namespace tether::agent {

DisplayManager::DisplayManager(const char*) {
    // TODO: XOpenDisplay，检查 XRandR 扩展版本 >= 1.2
}

DisplayManager::~DisplayManager() = default;

proto::DisplayLayout DisplayManager::layout() {
    // TODO: XRRGetScreenResources + XRRGetCrtcInfo，填 screens
    return {};
}

Status DisplayManager::resize(std::uint32_t, int, int) {
    // TODO
    return fail(Errc::Unsupported);
}

void DisplayManager::restore() {
    // TODO
}

bool DisplayManager::poll_changed() {
    // TODO: XRRSelectInput 订阅 RRScreenChangeNotifyMask，这里检查事件
    return false;
}

}  // namespace tether::agent
