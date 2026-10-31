// tether.transport:base  传输层抽象接口
//
// 上层（agent/client 的 session）只通过这个接口收发消息，
// 不关心底下是 SSH 的 stdin/stdout 还是 QUIC。
//
// 三个实现：
//   StdioTransport   SSH 的 stdin/stdout，全部消息走一个管道（MVP 只用这个）
//   QuicTransport    QUIC 数据通道，按 Channel 分流（模块 tether.transport.quic）
//   HybridTransport  组合前两个：控制走 SSH，其余优先走 QUIC，QUIC 断了自动退回 SSH
//
// Channel 表示消息的性质，各实现按它决定怎么发：
//   QUIC 下：Video/Audio 每帧一条单向流（过时就 RESET 掉），
//            Control/Input 各一条可靠流，InputLossy 走 QUIC datagram

export module tether.transport:base;

import tether.io;

export namespace tether {

enum class Channel {
    Control,      // 握手、ping、配置、剪贴板：可靠、有序
    Video,        // 视频帧：过时可丢
    Audio,        // 音频包：过时可丢，但不能乱序
    Input,        // 键盘、鼠标按键、滚轮：可靠、有序
    InputLossy,   // 鼠标移动：丢了无所谓，下一个包就是新位置
};

class Transport {
public:
    virtual ~Transport() = default;

    // 线程安全
    virtual Status send(Channel ch, const Message& msg) = 0;

    // 阻塞直到收到一条消息；对端正常关闭返回 Errc::Closed。只由一个线程调用
    virtual Result<Message> recv() = 0;

    // 关闭连接，让阻塞中的 recv() 尽快返回
    virtual void close() = 0;
};

}  // namespace tether
