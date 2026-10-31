// tether.util:error  统一的错误类型
//
// C++23 的 std::expected<T, E>：要么装着结果 T，要么装着错误 E。
// 用它代替异常来报告"预料之中"的失败（对端断开、数据格式不对、系统调用失败）：
//
//   Result<Message> recv();
//
//   auto m = reader.recv();
//   if (!m) {                                   // 失败
//       if (m.error().code == Errc::Closed) ... // 对端正常关闭
//       log.error("recv: {}", m.error().message());
//       return;
//   }
//   use(*m);                                    // 成功，*m 就是 Message
//
// 返回 void 的函数用 Status（= Result<void>），成功时 return {};
// 失败时 return std::unexpected(Error{...});，或者用下面的 fail() 简写。
//
// 参考：cppreference 上的 std::expected 页面（有完整示例）
//       tigervnc/common/core/Exception.h（TigerVNC 用异常，可以对比两种风格）

module;

#include <expected>
#include <string>

export module tether.util:error;

export namespace tether {

enum class Errc {
    Closed,        // 对端正常关闭（读到 EOF）。不算真正的错误
    Truncated,     // 消息读到一半连接断了
    Malformed,     // 数据格式不对：长度不够、长度超限、type 不匹配、有多余字节
    TooLarge,      // 消息或字段超过上限
    System,        // 系统调用失败，看 Error::sys（errno）
    Unsupported,   // 对端要求的功能本端不支持
};

struct Error {
    Errc code = Errc::System;
    int sys = 0;               // errno，只有 code == System 时有意义
    std::string detail;        // 补充说明，比如 "read" "need 4 bytes, have 2"

    // 给人看的完整描述，日志里用
    std::string message() const;
};

template <typename T>
using Result = std::expected<T, Error>;

using Status = Result<void>;

// return fail(Errc::Malformed, "bad length"); 的简写
inline std::unexpected<Error> fail(Errc code, std::string detail = {}, int sys = 0) {
    return std::unexpected(Error{code, sys, std::move(detail)});
}

}  // namespace tether
