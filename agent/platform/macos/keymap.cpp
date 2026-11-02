#include "platform/macos/keymap.h"

// #include <Carbon/Carbon.h>          // kVK_*
// #include <CoreGraphics/CoreGraphics.h>   // kCGEventFlagMask*

namespace tether::agent::macos_backend {

std::uint16_t evdev_to_cgkeycode(std::uint32_t) {
    // TODO: 查表。KEY_A(30) -> kVK_ANSI_A(0x00)，KEY_ESC(1) -> kVK_Escape(0x35)，
    //       KEY_LEFTMETA(125) -> kVK_Command(0x37)，KEY_LEFTALT(56) -> kVK_Option(0x3A) ...
    //       evdev 和 kVK 的顺序完全不同，只能逐个写
    return kNoKeyCode;
}

std::uint16_t keysym_to_cgkeycode(std::uint32_t) {
    // TODO: 只处理功能键、方向键、修饰键。XK_Return(0xff0d) -> kVK_Return ...
    return kNoKeyCode;
}

std::uint64_t modifier_flag_for_cgkeycode(std::uint16_t) {
    // TODO: kVK_Shift / kVK_RightShift -> kCGEventFlagMaskShift，
    //       kVK_Control -> kCGEventFlagMaskControl，kVK_Option -> kCGEventFlagMaskAlternate，
    //       kVK_Command -> kCGEventFlagMaskCommand，kVK_CapsLock -> kCGEventFlagMaskAlphaShift
    return 0;
}

}  // namespace tether::agent::macos_backend
