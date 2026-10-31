#include "input/keymap.h"

namespace tether::agent {

std::uint32_t modifier_keysym_to_evdev(std::uint32_t) {
    // TODO: XK_Shift_L -> KEY_LEFTSHIFT, XK_Control_L -> KEY_LEFTCTRL ...
    return 0;
}

}  // namespace tether::agent
