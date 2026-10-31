// tether.util:thread_queue  线程安全队列
// 参考：Sunshine/src/thread_safe.h（queue_t、mail_raw_t）
//
// 用途：线程之间传数据。比如 client 的接收线程把解码好的帧交给主线程渲染。
//
// 和 C++20 的 std::jthread 配合：
//   std::jthread worker([&](std::stop_token st) {
//       while (auto item = queue.pop(st)) { ... }   // request_stop() 后 pop 返回 nullopt
//   });
//   // jthread 析构时自动 request_stop() + join()，不用自己写 stop 标志
//
// 要点：
//   - std::condition_variable_any 的 wait(lock, stop_token, pred) 能同时等"有数据"和"被要求停止"
//   - max_size > 0 时，push 超过上限就丢掉最旧的元素（视频帧只关心最新的，
//     max_size = 1 就是"只保留最新一帧"）
//   - close() 用于没有 stop_token 的场景：之后所有 pop 立刻返回 nullopt
//
// 模板必须在接口里给出完整实现。
// 测试：tests/test_thread_queue.cpp

module;

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <stop_token>

export module tether.util:thread_queue;

export namespace tether {

template <typename T>
class ThreadQueue {
public:
    explicit ThreadQueue(std::size_t max_size = 0) : max_size_(max_size) {}

    void push(T item) {
        // TODO
        (void)item;
    }

    // 阻塞直到有数据、st 被要求停止、或 close()；后两种返回 std::nullopt
    std::optional<T> pop(std::stop_token st = {}) {
        // TODO
        (void)st;
        return std::nullopt;
    }

    // 不阻塞，没数据立刻返回 std::nullopt
    std::optional<T> try_pop() {
        // TODO
        return std::nullopt;
    }

    void close() {
        // TODO
    }

    std::size_t size() const {
        // TODO
        return 0;
    }

private:
    std::size_t max_size_;
    mutable std::mutex mutex_;
    std::condition_variable_any cv_;
    std::deque<T> items_;
    bool closed_ = false;
};

}  // namespace tether
