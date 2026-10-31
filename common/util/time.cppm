// tether.util:time  计时工具
// 参考：tigervnc/common/core/time.h、time.cxx
//
// 要点：
//   - 用 std::chrono::steady_clock（单调时钟）。system_clock 会因为改系统时间、
//     NTP 校时而跳变，拿来算帧间隔和 RTT 会出错
//   - 对外统一用 uint64_t 微秒，方便直接塞进协议消息
//
// 都是很短的 inline 函数，直接写在接口里。

module;

#include <chrono>
#include <cstdint>

export module tether.util:time;

export namespace tether {

// 某个固定起点之后经过的微秒数
inline std::uint64_t now_us() {
    // TODO
    return 0;
}

inline std::uint64_t now_ms() { return now_us() / 1000; }

// 秒表：构造时开始计时
class Stopwatch {
public:
    Stopwatch() : start_(now_us()) {}
    void reset() { /* TODO */ }
    std::uint64_t elapsed_us() const { /* TODO */ return 0; }

private:
    std::uint64_t start_;
};

// 帧率统计：每帧调一次 tick()，每满 1 秒算一次 fps
class FpsCounter {
public:
    // 返回 true 表示刚算出新的 fps
    bool tick() {
        // TODO
        (void)window_start_;
        (void)frames_;
        return false;
    }
    double fps() const { return fps_; }

private:
    std::uint64_t window_start_ = 0;
    std::uint64_t frames_ = 0;
    double fps_ = 0.0;
};

}  // namespace tether
