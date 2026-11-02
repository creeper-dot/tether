#include "platform/windows/keymap.h"

namespace tether::agent::windows_backend {

WinScancode evdev_to_win_scancode(std::uint32_t) {
    // TODO: 1~83 直接返回 {evdev, false}；扩展键查表
    //       KEY_KPENTER(96) -> {0x1C, true}，KEY_RIGHTCTRL(97) -> {0x1D, true}，
    //       KEY_RIGHTALT(100) -> {0x38, true}，KEY_HOME(102) -> {0x47, true}，KEY_UP(103) -> {0x48, true} ...
    return {};
}

std::uint16_t keysym_to_vk(std::uint32_t) {
    // TODO: 'a'~'z' -> 'A'~'Z'（VK 码就是大写 ASCII），'0'~'9' 不变，
    //       XK_Return(0xff0d) -> VK_RETURN，XK_Escape -> VK_ESCAPE，XK_F1 -> VK_F1 ...
    return 0;
}

bool keysym_to_unicode(std::uint32_t, char32_t&) {
    // TODO: (keysym & 0xff000000) == 0x01000000 时取低 24 位；
    //       0x20~0x7e、0xa0~0xff 的 keysym 本身就是 Latin-1 字符
    return false;
}

}  // namespace tether::agent::windows_backend
