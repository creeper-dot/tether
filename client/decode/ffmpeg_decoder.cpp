// H.264 / HEVC 解码（FFmpeg libavcodec）
//
// 参考（按阅读顺序）：
//   FFmpeg 源码 doc/examples/decode_video.c     最小解码示例，先看这个
//   FFmpeg 源码 doc/examples/hw_decode.c        硬件解码（VAAPI 等）示例
//   moonlight-qt/app/streaming/video/ffmpeg.cpp
//     低延迟参数设置、硬解失败退回软解、解码出错后请求关键帧
//
// 低延迟设置：
//   codec_ctx->flags  |= AV_CODEC_FLAG_LOW_DELAY
//   codec_ctx->flags2 |= AV_CODEC_FLAG2_FAST
//   thread_type = FF_THREAD_SLICE（帧级多线程会多攒几帧，增加延迟）
//
// 要点：
//   - 每个 VideoFrame.data 是一个完整的 Annex-B 访问单元，直接塞进 AVPacket
//   - avcodec_send_packet 之后马上 avcodec_receive_frame，应该一进一出
//   - 解码出错（丢了关键帧、数据损坏）返回 false，session 会请求关键帧
//   - 硬解出来的帧在显存里，要么 av_hwframe_transfer_data 拷回内存（简单），
//     要么直接交给渲染器零拷贝（moonlight 的 ffmpeg-renderers/ 做的就是这个，复杂）

#include "decode/decoder.h"

// extern "C" {
// #include <libavcodec/avcodec.h>
// }

namespace tether::client {

Result<std::unique_ptr<Decoder>> create_ffmpeg_decoder(proto::Codec, bool) {
    // TODO: 定义 class FfmpegDecoder : public Decoder
    return fail(Errc::Unsupported, "ffmpeg decoder not implemented");
}

}  // namespace tether::client
