#include "input/input_capture.h"

// #include <SDL.h>

namespace tether::client {

void InputCapture::set_remote_size(int w, int h) {
    remote_w_ = w;
    remote_h_ = h;
}

void InputCapture::set_viewport(const Viewport& vp) { viewport_ = vp; }

LocalAction InputCapture::handle(const SDL_Event&, std::vector<OutgoingInput>&) {
    // TODO: switch (ev.type)
    //   SDL_MOUSEMOTION          -> 记下位置，等 flush 再发
    //   SDL_MOUSEBUTTONDOWN/UP   -> MouseButton（Channel::Input）
    //   SDL_MOUSEWHEEL           -> MouseScroll（Channel::Input）
    //   SDL_KEYDOWN/UP           -> 先检查本地快捷键，再转 Key（Channel::Input）
    //   SDL_WINDOWEVENT 失焦     -> ReleaseAllKeys
    return LocalAction::None;
}

void InputCapture::flush(std::vector<OutgoingInput>&) {
    // TODO: mouse_moved_ 为 true 时发一个 MouseMove（Channel::InputLossy）
}

}  // namespace tether::client
