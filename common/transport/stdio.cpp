// tether.transport:stdio 的实现
module tether.transport;

namespace tether {

StdioTransport::StdioTransport(int in_fd, int out_fd)
    : in_fd_(in_fd), out_fd_(out_fd), reader_(in_fd), writer_(out_fd) {}

StdioTransport::~StdioTransport() = default;

Status StdioTransport::send(Channel, const Message& msg) {
    // TODO: return writer_.send(msg);（简单到可以直接写完，但留给你确认一下错误怎么传）
    (void)msg;
    return {};
}

Result<Message> StdioTransport::recv() {
    // TODO: return reader_.recv();
    return fail(Errc::Closed);
}

void StdioTransport::close() {
    // TODO: 关闭 out_fd_（对端会读到 EOF）。思考：in_fd_ 要不要也关？
    (void)in_fd_;
    (void)out_fd_;
}

}  // namespace tether
