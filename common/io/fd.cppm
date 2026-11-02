// tether.io:fd  管道 / 标准输入输出的读写
// 参考：tigervnc/common/rdr/FdInStream.cxx、FdOutStream.cxx
//
// 跨平台：句柄类型用 NativeHandle，接口里不出现平台头文件。
//   POSIX    NativeHandle = int（文件描述符），实现在 fd_posix.cpp（read/write）
//   Windows  NativeHandle = void*（就是 HANDLE），实现在 fd_win.cpp（ReadFile/WriteFile）
// CMake 按平台只编译其中一个实现文件。
//
// read()/write() 有两个坑，这里统一处理：
//   1. 短读/短写：read(fd, buf, 100) 可能只返回 30，不是错误，要循环到够数
//   2. EINTR：被信号打断时返回 -1 且 errno == EINTR，应该重试（Windows 没有这个问题）
//
// 返回值：
//   read_exact  一个字节都没读到就 EOF -> Errc::Closed（对端正常关闭）
//               读到一半 EOF          -> Errc::Truncated
//               系统调用失败          -> Errc::System，sys = errno / GetLastError()
//   write_all   失败                  -> Errc::System（对端关闭时 POSIX 是 EPIPE，
//                                        Windows 是 ERROR_BROKEN_PIPE / ERROR_NO_DATA）
//
// 测试：tests/test_framing.cpp 会用 pipe() 测这里

module;

#include <cstdint>
#include <span>

export module tether.io:fd;

import tether.util;

export namespace tether {

#ifdef _WIN32
using NativeHandle = void*;                       // HANDLE
inline const NativeHandle kInvalidHandle =
    reinterpret_cast<void*>(static_cast<std::intptr_t>(-1));    // INVALID_HANDLE_VALUE
#else
using NativeHandle = int;
inline constexpr NativeHandle kInvalidHandle = -1;
#endif

Status read_exact(NativeHandle h, std::span<std::uint8_t> buf);
Status write_all(NativeHandle h, std::span<const std::uint8_t> buf);

// 本进程的标准输入 / 输出（agent 用它们和 sshd 通信）
// POSIX 返回 0 / 1；Windows 返回 GetStdHandle(STD_INPUT_HANDLE / STD_OUTPUT_HANDLE)
NativeHandle std_input();
NativeHandle std_output();

// 关闭句柄（POSIX close，Windows CloseHandle）
void close_handle(NativeHandle h);

// 忽略 SIGPIPE：对端关闭后再写，默认会被这个信号直接杀死且没有任何提示。
// 忽略后 write 返回 EPIPE，可以正常处理。main() 开头调一次。
// Windows 没有 SIGPIPE，这个函数什么都不做。
void ignore_sigpipe();

}  // namespace tether
