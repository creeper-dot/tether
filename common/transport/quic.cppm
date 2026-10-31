// tether.transport.quic  QUIC 数据通道（在 MVP 和 H.264 都跑通之后再做）
//
// 单独成一个模块，因为它依赖 msquic，是可选编译的（TETHER_WITH_QUIC）。
//
// 库：msquic（微软，C API，文档全）https://github.com/microsoft/msquic
//     备选 ngtcp2 + quictls、quiche（Cloudflare，Rust 写的 C API）
// 参考：
//   msquic 仓库 src/tools/sample/sample.c      最小的 client/server 示例，先跑通它
//   msquic 仓库 docs/API.md                    流、datagram、连接的 API 说明
//   RFC 9000（QUIC）、RFC 9221（QUIC Datagram）
//   moonlight-qt/moonlight-common-c（需要 git submodule 拉下来）
//     看它在 UDP 上怎么处理视频丢包、请求关键帧，思路可以借鉴
//
// 设计：
//   Video/Audio：每帧开一条新的单向流，写完就 FIN。新帧发出后旧帧还没送达就
//                RESET 掉旧流。丢包只影响当前帧，不阻塞后面的帧
//   Control/Input：各一条长期存在的双向流，内部仍用 framing 的 5 字节头分帧
//   InputLossy：QUIC datagram，一个 datagram 一条消息
//
// 安全：
//   agent 每次启动生成自签证书，指纹通过 SSH 发给 client（UdpOffer）。
//   client 只接受这个指纹，不走 CA 验证。连上后第一条消息必须是正确的令牌。
//
// msquic 的头文件在模块实现单元（quic.cpp）的全局模块片段里 #include，
// 接口里不暴露任何 msquic 类型，使用方不需要 msquic 的头文件。

module;

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

export module tether.transport.quic;

import tether.transport;

export namespace tether {

struct QuicServerParams {
    std::uint16_t port_min = 60000;      // 在这个范围里找一个空闲端口
    std::uint16_t port_max = 60100;
};

struct QuicClientParams {
    std::string host;
    std::uint16_t port = 0;
    std::vector<std::uint8_t> cert_sha256;
    std::vector<std::uint8_t> token;
};

class QuicTransport final : public Transport {
public:
    // agent：监听，成功后可以拿到端口、指纹、令牌，填进 UdpOffer
    static Result<std::unique_ptr<QuicTransport>> listen(const QuicServerParams& p);
    // client：连接并校验
    static Result<std::unique_ptr<QuicTransport>> connect(const QuicClientParams& p);

    ~QuicTransport() override;

    Status send(Channel ch, const Message& msg) override;
    Result<Message> recv() override;
    void close() override;

    std::uint16_t port() const { return port_; }
    const std::vector<std::uint8_t>& cert_sha256() const { return cert_sha256_; }
    const std::vector<std::uint8_t>& token() const { return token_; }

private:
    QuicTransport() = default;

    std::uint16_t port_ = 0;
    std::vector<std::uint8_t> cert_sha256_;
    std::vector<std::uint8_t> token_;

    struct Impl;                    // msquic 句柄等，定义在 quic.cpp 里（pimpl 写法）
    std::unique_ptr<Impl> impl_;
};

}  // namespace tether
