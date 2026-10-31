#include "render/sdl_renderer.h"

// #include <SDL.h>

namespace tether::client {

SdlRenderer::SdlRenderer() = default;
SdlRenderer::~SdlRenderer() = default;

Status SdlRenderer::init(const char*, int, int) {
    // TODO: SDL_Init(SDL_INIT_VIDEO) -> SDL_CreateWindow -> SDL_CreateRenderer
    // 失败时 return fail(Errc::System, SDL_GetError());
    return fail(Errc::Unsupported);
}

void SdlRenderer::present(const DecodedFrame&) {
    // TODO: 尺寸或格式变了就重建纹理 -> SDL_UpdateTexture / SDL_UpdateYUVTexture
    //       -> SDL_RenderClear -> SDL_RenderCopy(dst = viewport) -> SDL_RenderPresent
}

void SdlRenderer::on_window_resized() {
    // TODO: 重算 viewport_
}

void SdlRenderer::set_fullscreen(bool) {
    // TODO: SDL_SetWindowFullscreen(SDL_WINDOW_FULLSCREEN_DESKTOP)
}

void SdlRenderer::window_size(int& w, int& h) const {
    // TODO: SDL_GetWindowSize
    w = 0;
    h = 0;
}

}  // namespace tether::client
