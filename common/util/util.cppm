// tether.util  主模块接口：把各分区拼起来
//
// 使用方写 import tether.util; 就能用到下面所有分区导出的东西。
// 新加分区时：新建 xxx.cppm（export module tether.util:xxx;），
// 在这里加一行 export import :xxx;，再在 common/CMakeLists.txt 里登记文件。

export module tether.util;

export import :error;
export import :log;
export import :time;
export import :thread_queue;
