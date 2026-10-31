// tether.transport  主模块接口
//
// import tether.transport; 之后能用到 Transport、Channel、StdioTransport、HybridTransport，
// 以及 tether.io / tether.util 的全部内容。
// QUIC 实现是单独的模块 tether.transport.quic（可选编译），需要时另外 import。

export module tether.transport;

export import tether.io;
export import :base;
export import :stdio;
export import :hybrid;
