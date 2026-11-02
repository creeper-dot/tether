// tether.util:log  日志
// 参考：tigervnc/common/core/LogWriter.h、Logger.h、Logger_stdio.cxx
//
// 用法：
//   static tether::Logger vlog("capture");
//   vlog.info("screen {}x{}", w, h);          // std::format 语法，类型不对时编译报错
//
// 要点：
//   - 只写 stderr。agent 的 stdout 是协议通道，往里打印会把协议数据搞乱
//   - 级别低于全局设置的直接不格式化（先判断 enabled 再 std::format，省 CPU）
//   - 多线程同时打日志时一行不能被打断
//   - 从环境变量 TETHER_LOG=debug 读级别
//
// 为什么 info() 等函数是模板、写在接口里：
//   std::format_string 要在编译期检查格式串，必须在调用方能看到完整定义。
//   真正写 stderr 的 write() 不是模板，放在 log.cpp 里实现。

module;

#include <format>
#include <string_view>

export module tether.util:log;

export namespace tether {

enum class LogLevel { Error = 0, Warn = 1, Info = 2, Debug = 3 };

class Logger {
public:
    constexpr explicit Logger(std::string_view name) noexcept : name_(name) {}

    template <typename... Args>
    auto error(std::format_string<Args...> fmt, Args&&... args) const noexcept -> void {
        log(LogLevel::Error, fmt.get(), std::make_format_args(args...));
    }
    template <typename... Args>
    auto warn(std::format_string<Args...> fmt, Args&&... args) const noexcept -> void {
        log(LogLevel::Warn, fmt.get(), std::make_format_args(args...));
    }
    template <typename... Args>
    auto info(std::format_string<Args...> fmt, Args&&... args) const noexcept -> void {
        log(LogLevel::Info, fmt.get(), std::make_format_args(args...));
    }
    template <typename... Args>
    auto debug(std::format_string<Args...> fmt, Args&&... args) const noexcept -> void {
        log(LogLevel::Debug, fmt.get(), std::make_format_args(args...));
    }

    static auto set_level(LogLevel level) noexcept -> void;

    [[nodiscard]] static auto level() noexcept -> LogLevel;

    static auto init_from_env() noexcept -> void;

private:
    auto log(LogLevel lv, std::string_view fmt, std::format_args args) const noexcept -> void {
        if (lv <= level()) {
            write(lv, fmt, args);
        }
    }

    auto write(LogLevel level, std::string_view fmt, std::format_args args) const noexcept -> void;

    std::string_view name_;
};

}  // namespace tether
