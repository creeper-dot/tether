// Windows 后端共用的小工具
//
// 只在 platform/windows/*.cpp 里 include，会带进 <windows.h>。

#pragma once

#include <expected>
#include <string>
#include <string_view>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

import tether.util;

namespace tether::agent::windows_backend {

// UTF-8 <-> UTF-16。协议里文本都是 UTF-8，Windows API（W 版本）要 UTF-16。
// 用 MultiByteToWideChar / WideCharToMultiByte，CP_UTF8；先传 0 长度问出需要多大，再真正转换
std::wstring utf8_to_wide(std::string_view s);
std::string wide_to_utf8(std::wstring_view s);

// 把当前线程切到"正在接收输入的桌面"。
//
// Windows 有多个桌面：普通桌面 Default、锁屏/UAC 弹窗用的 Winlogon 安全桌面。
// 线程只能截取、注入自己所在的桌面。用户锁屏或弹出 UAC 时输入桌面会切换，
// 这时 DXGI 返回 DXGI_ERROR_ACCESS_LOST，SendInput 返回 0，需要重新调这个函数再重建。
// 注意：切到安全桌面需要 SYSTEM 权限，普通用户进程只能在 Default 桌面上工作。
//
// 做法：OpenInputDesktop -> SetThreadDesktop -> CloseDesktop
// 截屏线程、注入线程都要各调一次（SetThreadDesktop 只影响调用它的线程）。
// 参考：Sunshine/src/platform/windows/misc.cpp  syncThreadDesktop()
Status sync_input_desktop();

// HRESULT 失败时转成 Error（code = System，sys = hr，detail = what）
// Error::sys 是 int，HRESULT 是 32 位 long，直接转换不会丢信息
inline std::unexpected<Error> fail_hr(const char* what, HRESULT hr) {
    return fail(Errc::System, what, static_cast<int>(hr));
}

// GetLastError() 转成 Error
inline std::unexpected<Error> fail_last_error(const char* what) {
    return fail(Errc::System, what, static_cast<int>(GetLastError()));
}

}  // namespace tether::agent::windows_backend
