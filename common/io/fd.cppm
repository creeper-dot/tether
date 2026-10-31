// tether.io:fd  文件描述符读写
// 参考：tigervnc/common/rdr/FdInStream.cxx、FdOutStream.cxx
//
// read()/write() 有两个坑，这里统一处理：
//   1. 短读/短写：read(fd, buf, 100) 可能只返回 30，不是错误，要循环到够数
//   2. EINTR：被信号打断时返回 -1 且 errno == EINTR，应该重试
//
// 返回值：
//   read_exact  一个字节都没读到就 EOF -> Errc::Closed（对端正常关闭）
//               读到一半 EOF          -> Errc::Truncated
//               系统调用失败          -> Errc::System，sys = errno
//   write_all   失败                  -> Errc::System（对端关闭时 errno 是 EPIPE）
//
// 测试：tests/test_framing.cpp 会用 pipe() 测这里

module;

#include <cstdint>
#include <span>

export module tether.io:fd;

import tether.util;

export namespace tether {

Status read_exact(int fd, std::span<std::uint8_t> buf);
Status write_all(int fd, std::span<const std::uint8_t> buf);

// 忽略 SIGPIPE：对端关闭后再写，默认会被这个信号直接杀死且没有任何提示。
// 忽略后 write 返回 EPIPE，可以正常处理。main() 开头调一次。
void ignore_sigpipe();

}  // namespace tether
