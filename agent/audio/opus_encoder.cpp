#include "audio/opus_encoder.h"

// #include <opus/opus.h>

namespace tether::agent {

OpusEncoder::OpusEncoder() = default;
OpusEncoder::~OpusEncoder() = default;

Status OpusEncoder::init(std::uint32_t, int, int) {
    // TODO: opus_encoder_create + opus_encoder_ctl(OPUS_SET_BITRATE)
    return fail(Errc::Unsupported);
}

Status OpusEncoder::encode(const std::vector<float>&, std::vector<std::uint8_t>&) {
    // TODO: opus_encode_float
    return fail(Errc::Unsupported);
}

}  // namespace tether::agent
