#include "platform/windows/win_util.h"

namespace tether::agent::windows_backend {

std::wstring utf8_to_wide(std::string_view) {
    // TODO: MultiByteToWideChar(CP_UTF8, 0, ...) 两次：第一次拿长度，第二次写入
    return {};
}

std::string wide_to_utf8(std::wstring_view) {
    // TODO: WideCharToMultiByte(CP_UTF8, 0, ...) 两次
    return {};
}

Status sync_input_desktop() {
    // TODO: HDESK d = OpenInputDesktop(0, FALSE, GENERIC_ALL)，失败 return fail_last_error(...)
    //       SetThreadDesktop(d)，然后 CloseDesktop(d)
    return fail(Errc::Unsupported, "sync_input_desktop not implemented");
}

}  // namespace tether::agent::windows_backend
