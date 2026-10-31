// tether.transport.quic 的实现
module;

#include <memory>
// #include <msquic.h>

module tether.transport.quic;

namespace tether {

struct QuicTransport::Impl {
    // TODO: HQUIC registration、configuration、connection、listener ...
};

Result<std::unique_ptr<QuicTransport>> QuicTransport::listen(const QuicServerParams&) {
    // TODO
    return fail(Errc::Unsupported, "quic not implemented");
}

Result<std::unique_ptr<QuicTransport>> QuicTransport::connect(const QuicClientParams&) {
    // TODO
    return fail(Errc::Unsupported, "quic not implemented");
}

QuicTransport::~QuicTransport() = default;

Status QuicTransport::send(Channel, const Message&) {
    // TODO
    return fail(Errc::Unsupported);
}

Result<Message> QuicTransport::recv() {
    // TODO
    return fail(Errc::Closed);
}

void QuicTransport::close() {
    // TODO
}

}  // namespace tether
