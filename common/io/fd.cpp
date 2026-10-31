// tether.io:fd 的实现
module;

#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <span>
#include <unistd.h>

module tether.io;

namespace tether {

Status read_exact(int, std::span<std::uint8_t>) {
    // TODO: 循环 ::read，处理 EINTR、短读、EOF
    return {};
}

Status write_all(int, std::span<const std::uint8_t>) {
    // TODO: 循环 ::write，处理 EINTR、短写
    return {};
}

void ignore_sigpipe() {
    // TODO: std::signal(SIGPIPE, SIG_IGN)
}

}  // namespace tether
