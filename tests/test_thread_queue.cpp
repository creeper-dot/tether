// 测试 tether.util 的 thread_queue 分区

#include <stop_token>
#include <thread>

#include "check.h"

import tether.util;

static void test_fifo() {
    tether::ThreadQueue<int> q;
    q.push(1);
    q.push(2);
    CHECK(q.size() == 2);
    CHECK(q.pop() == 1);
    CHECK(q.pop() == 2);
}

static void test_stop_wakes_pop() {
    tether::ThreadQueue<int> q;
    bool got_nullopt = false;
    {
        std::jthread t([&](std::stop_token st) { got_nullopt = !q.pop(st).has_value(); });
        // 离开作用域时 jthread 析构：request_stop() + join()。
        // 如果 pop 没有响应 stop_token，这里会永远卡住
    }
    CHECK(got_nullopt);
}

// TODO: 自己补充
//   - try_pop() 在空队列上立刻返回 nullopt
//   - max_size = 1 时连续 push 三次，只剩最后一个
//   - close() 之后 pop() 立刻返回 nullopt
//   - 一个线程 push 10000 个数，另一个线程 pop，收到的顺序和总数都对

int main() {
    test_fifo();
    test_stop_wakes_pop();
    return test_result();
}
