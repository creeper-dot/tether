#include "cursor/cursor_renderer.h"

// #include <SDL.h>

namespace tether::client {

CursorRenderer::CursorRenderer() = default;

CursorRenderer::~CursorRenderer() {
    // TODO: SDL_FreeCursor
}

void CursorRenderer::set_shape(const proto::CursorShape&, float) {
    // TODO
}

void CursorRenderer::set_visible(bool) {
    // TODO: SDL_ShowCursor
}

}  // namespace tether::client
