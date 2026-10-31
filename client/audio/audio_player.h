// 声音播放：Opus 解码 + SDL 音频输出
//
// 参考：
//   moonlight-qt/app/streaming/audio/audio.cpp
//     第 75 行附近：opus_multistream_decoder_create（立体声用 opus_decoder_create 就行）
//   moonlight-qt/app/streaming/audio/renderers/sdlaud.cpp
//     第 40 行附近：SDL_OpenAudioDevice
//     第 127 行附近：SDL_QueueAudio，以及队列太长时丢掉旧数据控制延迟
//   tigervnc/vncviewer/AudioOutputPulse.cxx   用 PulseAudio 播放的另一种做法
//
// 要点：
//   - 网络抖动会让包到达时间不均匀，需要一个小的抖动缓冲（20~40ms）
//   - 缓冲积压超过上限（比如 100ms）就丢掉一部分，保证延迟不累积
//   - 丢包时调 opus_decode(nullptr, ...) 让 Opus 自己补一段（PLC）

#pragma once

import tether.protocol;

namespace tether::client {

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();

    Status start(const proto::AudioConfig& cfg);

    // 接收线程调用
    void on_packet(const proto::AudioPacket& pkt);

    void stop();

private:
    // TODO: OpusDecoder*、SDL_AudioDeviceID
};

}  // namespace tether::client
