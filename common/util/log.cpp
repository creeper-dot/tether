// tether.util:log 的实现
module;

#include <cstdio>
#include <string_view>

module tether.util;

namespace tether {

void Logger::write(LogLevel, std::string_view) const {
    // TODO: 拼"时间戳 级别 模块名 消息"，用 std::print(stderr, ...) 一次写出
    //       多线程同时写时一行不能被打断：加一把全局 mutex
}

void Logger::set_level(LogLevel) {
    // TODO
}

LogLevel Logger::level() {
    // TODO
    return LogLevel::Info;
}

void Logger::init_from_env() {
    // TODO: 读 std::getenv("TETHER_LOG")（需要 #include <cstdlib>）
}

}  // namespace tether
