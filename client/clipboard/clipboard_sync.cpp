#include "clipboard/clipboard_sync.h"

// #include <SDL.h>

namespace tether::client {

std::optional<proto::Clipboard> ClipboardSync::on_local_change() {
    // TODO: SDL_GetClipboardText（记得 SDL_free），和 last_set_ 比较
    return std::nullopt;
}

void ClipboardSync::on_remote(const proto::Clipboard&) {
    // TODO: SDL_SetClipboardText，记下 last_set_
}

}  // namespace tether::client
