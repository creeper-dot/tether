// 剪贴板同步（client 侧）
//
// client 用 SDL 的剪贴板 API，跨平台，比 agent 侧简单很多。
//
// 参考：
//   tigervnc/vncviewer/Viewport.cxx
//     第 112 行附近：订阅本地剪贴板变化
//     第 263-295 行：处理远端剪贴板请求、通知、数据
//   SDL2 文档：SDL_GetClipboardText / SDL_SetClipboardText / SDL_CLIPBOARDUPDATE 事件
//
// 要点：
//   - 本地剪贴板变化（SDL_CLIPBOARDUPDATE）-> 发 proto::Clipboard
//   - 收到远端的 Clipboard -> SDL_SetClipboardText
//   - 防回环：自己刚设置的内容触发的 SDL_CLIPBOARDUPDATE 不要再发回去
//   - 可以加个开关：只同步一个方向，或者完全关闭（安全考虑）

#pragma once

#include <optional>
#include <string>

import tether.protocol;

namespace tether::client {

class ClipboardSync {
public:
    // 收到 SDL_CLIPBOARDUPDATE 时调用；需要发给远端就返回消息
    std::optional<proto::Clipboard> on_local_change();

    // 收到远端 Clipboard 消息时调用
    void on_remote(const proto::Clipboard& msg);

private:
    std::string last_set_;   // 防回环
};

}  // namespace tether::client
