// tether.util:log 的实现
module;

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iterator>
#include <mutex>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

module tether.util;

namespace tether {

namespace {

constinit std::atomic<LogLevel> log_level{LogLevel::Info};
constinit std::mutex log_mutex;

constexpr auto log_level_name(LogLevel level) noexcept -> std::string_view {
    using enum LogLevel;
    switch (level) {
        case Error: return "Error";
        case Warn:  return "Warn";
        case Info:  return "Info";
        case Debug: return "Debug";
    }
    return "Unknown";
}

constexpr auto to_lower(char c) noexcept -> char {
    return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
}

constexpr auto iequals(std::string_view a, std::string_view b) noexcept -> bool {
    return std::ranges::equal(a, b, {}, to_lower, to_lower);
}

constexpr auto parse_log_level(std::string_view s) noexcept -> std::optional<LogLevel> {
    using enum LogLevel;
    for (auto const lv : {Error, Warn, Info, Debug}) {
        if (iequals(s, log_level_name(lv))) {
            return lv;
        }
    }

    int n = 0;
    auto const* const end = s.data() + s.size();
    auto const [ptr, ec] = std::from_chars(s.data(), end, n);
    if (ec == std::errc{} && ptr == end && n >= std::to_underlying(Error) && n <= std::to_underlying(Debug)) {
        return static_cast<LogLevel>(n);
    }
    return std::nullopt;
}

}  // namespace

auto Logger::write(LogLevel lv, std::string_view fmt, std::format_args args) const noexcept -> void {
    // 调用方常在系统调用失败后先打日志再读 errno，内存分配和写 stderr 都可能改掉它
    auto const saved_errno = errno;

    // 分配失败、自定义 formatter 抛异常、写 stderr 失败都在这里吞掉，日志不能把异常抛给调用方
    try {
        auto const now = std::chrono::floor<std::chrono::milliseconds>(
            std::chrono::system_clock::now()
        );

        auto line = std::format("{:%T} {:<5} {}: ", now, log_level_name(lv), name_);
        std::vformat_to(std::back_inserter(line), fmt, args);
        if (!line.ends_with('\n')) {
            line.push_back('\n');
        }

        std::scoped_lock lock(log_mutex);
        std::print(stderr, "{}", line);
    } catch (...) {
    }

    errno = saved_errno;
}

auto Logger::set_level(LogLevel level) noexcept -> void {
    log_level.store(level, std::memory_order_relaxed);
}

auto Logger::level() noexcept -> LogLevel {
    return log_level.load(std::memory_order_relaxed);
}

auto Logger::init_from_env() noexcept ->  void {
    auto const* const env = std::getenv("TETHER_LOG");
    if (env == nullptr || *env == '\0') {
        return;
    }
    if (auto const lv = parse_log_level(env)) {
        set_level(*lv);
    } else {
        Logger("log").warn("unknown TETHER_LOG value '{}', using {}", env, log_level_name(level()));
    }
}

}  // namespace tether
