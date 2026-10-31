// 启动 ssh 子进程，拿到它的 stdin/stdout
//
// 效果等同于在终端执行：ssh user@host tether-agent --stdio
// 我们往 ssh 的 stdin 写 = 发给远端 agent；从 ssh 的 stdout 读 = 收 agent 的数据。
// ssh 的 stderr 不接管，直接输出到当前终端（密码提示、agent 日志都能看到）。
//
// 实现步骤（经典的 pipe + fork + exec）：
//   1. pipe() 两次，得到 to_child[2]、from_child[2]
//   2. fork()
//   3. 子进程：dup2 把管道接到 0/1 -> 关掉多余的 fd -> execvp("ssh", argv)
//   4. 父进程：关掉子进程那一端，保留 to_child[1] 写、from_child[0] 读
//
// 也可以用 posix_spawn 代替 fork+exec，更安全一些。
// 注意：argv 直接传给 execvp，不经过 shell，host 名里有空格、引号也不会被注入。
//
// 本地测试：把 argv 换成 {"./tether-agent", "--stdio"}，不经过 ssh 直接跑。
//
// 参考：
//   man 2 pipe、man 2 dup2、man 3 posix_spawn
//   mosh 源码 scripts/mosh.pl          mosh 怎么通过 ssh 启动远端 mosh-server 并拿回端口和密钥
//   tigervnc/vncviewer/vncviewer.cxx   搜 "via"，TigerVNC 的 -via 选项用 ssh 建隧道
//
// ssh 参数建议：
//   -T                  不分配终端（否则 ssh 会改写 \n，破坏二进制数据）
//   -o Compression=no   视频已经压缩过，再压只会浪费 CPU
//   -e none             关掉 ~ 转义字符，否则数据里的 "\n~." 会被 ssh 当成断开命令

#pragma once

#include <string>
#include <sys/types.h>
#include <vector>

import tether.util;

namespace tether::client {

class SshProcess {
public:
    SshProcess() = default;
    ~SshProcess();

    SshProcess(const SshProcess&) = delete;
    SshProcess& operator=(const SshProcess&) = delete;

    // argv 例：{"ssh", "user@host", "tether-agent", "--stdio"}
    Status start(const std::vector<std::string>& argv);

    int read_fd() const { return read_fd_; }    // 读 = agent 的 stdout
    int write_fd() const { return write_fd_; }  // 写 = agent 的 stdin

    // 等子进程退出，返回退出码
    int wait();

private:
    pid_t pid_ = -1;
    int read_fd_ = -1;
    int write_fd_ = -1;
};

}  // namespace tether::client
