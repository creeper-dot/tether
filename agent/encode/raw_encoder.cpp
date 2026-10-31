// "编码器"：MVP 阶段直接把 BGRA 原样拷贝出去（Codec::Raw）
//
// 目的是先把整条链路跑通。1080p 一帧约 8MB，本机/局域网能跑，跨网络很卡，正常。
// 链路通了以后换成 ffmpeg_encoder。

#include "encode/encoder.h"

namespace tether::agent {

std::unique_ptr<Encoder> create_raw_encoder() {
    // TODO: 定义 class RawEncoder : public Encoder
    //       encode() 返回 Status，里面按 stride 逐行拷贝（去掉行尾填充），codec = Codec::Raw
    return nullptr;  // 不压缩，不会失败，所以直接返回指针而不是 Result
}

}  // namespace tether::agent
