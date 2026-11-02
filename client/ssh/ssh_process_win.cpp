// SshProcess 的 Windows 实现
//
// Windows 10 1809+ 自带 OpenSSH 客户端（C:\Windows\System32\OpenSSH\ssh.exe），参数和 Linux 一样。
//
// 实现步骤：
//   1. CreatePipe 两次，得到 to_child、from_child；SECURITY_ATTRIBUTES.bInheritHandle = TRUE
//   2. 父进程那一端用 SetHandleInformation 去掉 HANDLE_FLAG_INHERIT，否则子进程也会继承它，
//      父进程关闭写端后子进程读不到 EOF
//   3. STARTUPINFOW 里 dwFlags = STARTF_USESTDHANDLES，hStdInput / hStdOutput 设成子进程那一端，
//      hStdError = GetStdHandle(STD_ERROR_HANDLE)（密码提示、agent 日志照常显示）
//   4. CreateProcessW 启动，之后关掉子进程那一端
//   更稳妥的做法：用 STARTUPINFOEXW + PROC_THREAD_ATTRIBUTE_HANDLE_LIST 只让子进程继承这两个句柄
//
// 注意：
//   - Windows 没有 argv 数组，CreateProcessW 要的是一整条命令行字符串。
//     每个参数要按 CommandLineToArgvW 的规则转义（空格、引号、反斜杠），否则 host 名里的
//     特殊字符会被拆错。参考 "Everyone quotes command line arguments the wrong way"（MSDN 博客）
//   - 参数是 UTF-8，要用 MultiByteToWideChar 转成 UTF-16
//
// 参考：learn.microsoft.com "Creating a Child Process with Redirected Input and Output"

#include "ssh/ssh_process.h"

namespace tether::client {

SshProcess::~SshProcess() {
    // TODO: close_handle 两个管道句柄；WaitForSingleObject 等进程退出后 CloseHandle(process_)
}

Status SshProcess::start(const std::vector<std::string>&) {
    // TODO: CreatePipe 失败时 return fail(Errc::System, "CreatePipe", GetLastError());
    return fail(Errc::Unsupported);
}

int SshProcess::wait() {
    // TODO: WaitForSingleObject + GetExitCodeProcess
    return -1;
}

}  // namespace tether::client
