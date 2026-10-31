#include "audio/audio_capture.h"

// #include <pulse/simple.h>
// #include <pulse/error.h>

namespace tether::agent {

AudioCapture::AudioCapture() = default;
AudioCapture::~AudioCapture() = default;

Status AudioCapture::start(std::uint32_t, int) {
    // TODO: 找默认 sink 的 monitor -> pa_simple_new(PA_STREAM_RECORD)
    return fail(Errc::Unsupported);
}

Status AudioCapture::read(std::vector<float>&, int) {
    // TODO: pa_simple_read
    return fail(Errc::Unsupported);
}

void AudioCapture::stop() {
    // TODO: pa_simple_free
}

}  // namespace tether::agent
