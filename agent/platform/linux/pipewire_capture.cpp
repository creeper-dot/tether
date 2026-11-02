// Wayland 截屏：从 portal 给的 PipeWire 流里取画面（X11 版本稳定后再做）
//
// 参考：
//   Sunshine/src/platform/linux/pipewire.cpp     PipeWire 流的建立、格式协商、取 buffer
//   Sunshine/src/platform/linux/portalgrab.cpp   和 portal 配合的整体流程
//   PipeWire 官方教程 https://docs.pipewire.org/page_tutorial5.html（"capturing video frames"）
//   obs-studio 的 plugins/linux-pipewire/  另一份成熟实现，比 Sunshine 好读一些
//
// 要点：
//   - 用 portal 返回的 fd 调 pw_context_connect_fd，再用 node_id 连到视频流
//   - 格式协商：先只接受 BGRx/BGRA 的内存 buffer（SPA_DATA_MemPtr / MemFd），
//     DMA-BUF 零拷贝留到做硬件编码时再支持
//   - PipeWire 有自己的线程循环（pw_thread_loop），回调里拿到帧后拷贝出来，
//     grab() 从中取最新一帧
//   - 光标：可以请求 metadata 模式，光标单独给出，不画进画面

#include "platform/linux/backends.h"
#include "platform/linux/portal_session.h"

namespace tether::agent::linux_backend {

Result<std::unique_ptr<Capturer>> create_pipewire_capturer(PortalSession&) {
    // TODO
    return fail(Errc::Unsupported, "pipewire capture not implemented");
}

}  // namespace tether::agent::linux_backend
