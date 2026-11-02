// tether.io:fd 的 POSIX 实现（Linux / macOS）
module;

#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <span>
#include <unistd.h>

module tether.io;

namespace tether {

Status read_exact(NativeHandle, std::span<std::uint8_t>) {
    // TODO: 循环 ::read，处理 EINTR、短读、EOF
    return {};
}

Status write_all(NativeHandle, std::span<const std::uint8_t>) {
    // TODO: 循环 ::write，处理 EINTR、短写
    return {};
}

NativeHandle std_input() { return STDIN_FILENO; }
NativeHandle std_output() { return STDOUT_FILENO; }

void close_handle(NativeHandle) {
    // TODO: ::close，h == kInvalidHandle 时直接返回
}

void ignore_sigpipe() {
    // TODO: std::signal(SIGPIPE, SIG_IGN)
}

}  // namespace tether
