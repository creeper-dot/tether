#include "ssh/ssh_process.h"

namespace tether::client {

SshProcess::~SshProcess() {
    // TODO: 关闭 fd，waitpid 回收子进程（不然会留下僵尸进程）
}

Status SshProcess::start(const std::vector<std::string>&) {
    // TODO: pipe 失败时 return fail(Errc::System, "pipe", errno);
    return fail(Errc::Unsupported);
}

int SshProcess::wait() {
    // TODO: waitpid
    return -1;
}

}  // namespace tether::client
