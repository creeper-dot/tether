// 极简测试工具，不引入测试框架
//
//   CHECK(a == b);          条件不成立就打印文件名、行号并记一次失败
//   CHECK_THROWS(expr, T);  expr 应该抛出 T 类型的异常
//   return test_result();   main 最后调用，有失败返回 1

#pragma once

#include <cstdio>

namespace tether::test {
inline int& failures() {
    static int n = 0;
    return n;
}
}  // namespace tether::test

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, \
                    #cond);                                                  \
            ++tether::test::failures();                                          \
        }                                                                    \
    } while (0)

#define CHECK_THROWS(expr, ExType)                                             \
    do {                                                                       \
        bool thrown_ = false;                                                  \
        try {                                                                  \
            (void)(expr);                                                      \
        } catch (const ExType&) {                                              \
            thrown_ = true;                                                    \
        } catch (...) {                                                        \
        }                                                                      \
        if (!thrown_) {                                                        \
            fprintf(stderr, "%s:%d: expected %s from: %s\n", __FILE__,         \
                    __LINE__, #ExType, #expr);                                 \
            ++tether::test::failures();                                            \
        }                                                                      \
    } while (0)

inline int test_result() {
    int n = tether::test::failures();
    if (n == 0) fprintf(stderr, "OK\n");
    else fprintf(stderr, "%d check(s) failed\n", n);
    return n == 0 ? 0 : 1;
}
