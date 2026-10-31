#include "audio/audio_player.h"

// #include <SDL.h>
// #include <opus/opus.h>

namespace tether::client {

AudioPlayer::AudioPlayer() = default;
AudioPlayer::~AudioPlayer() = default;

Status AudioPlayer::start(const proto::AudioConfig&) {
    // TODO: opus_decoder_create + SDL_OpenAudioDevice
    return fail(Errc::Unsupported);
}

void AudioPlayer::on_packet(const proto::AudioPacket&) {
    // TODO: opus_decode_float -> 检查 SDL_GetQueuedAudioSize -> SDL_QueueAudio
}

void AudioPlayer::stop() {
    // TODO
}

}  // namespace tether::client
