// tether.io:fd 的 Windows 实现
//
// 要点：
//   - ReadFile 读管道时，对端关闭返回 FALSE 且 GetLastError() == ERROR_BROKEN_PIPE，
//     这相当于 POSIX 的 EOF（read 返回 0），要按 Closed / Truncated 处理，不算 System 错误
//   - WriteFile 对端关闭时是 ERROR_BROKEN_PIPE 或 ERROR_NO_DATA
//   - 单次 ReadFile/WriteFile 的长度是 DWORD，大 buffer 要分段
//   - Error::sys 里放 GetLastError() 的值
//
// 参考：learn.microsoft.com 上 ReadFile、WriteFile、GetStdHandle 的文档

module;

#include <cstddef>
#include <cstdint>
#include <span>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

module tether.io;

namespace tether {

Status read_exact(NativeHandle, std::span<std::uint8_t>) {
    // TODO: 循环 ReadFile，处理短读；ERROR_BROKEN_PIPE 当作 EOF
    return {};
}

Status write_all(NativeHandle, std::span<const std::uint8_t>) {
    // TODO: 循环 WriteFile，处理短写
    return {};
}

NativeHandle std_input() { return GetStdHandle(STD_INPUT_HANDLE); }
NativeHandle std_output() { return GetStdHandle(STD_OUTPUT_HANDLE); }

void close_handle(NativeHandle) {
    // TODO: CloseHandle，h == kInvalidHandle 或 nullptr 时直接返回
}

void ignore_sigpipe() {}

}  // namespace tether
