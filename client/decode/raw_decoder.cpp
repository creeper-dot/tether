#include "decode/decoder.h"

namespace tether::client {

std::unique_ptr<Decoder> create_raw_decoder() {
    // TODO: 定义 class RawDecoder : public Decoder
    //       检查 data.size() == width * height * 4，不对就 return fail(Errc::Malformed, ...)（对端数据不可信）
    return nullptr;  // 不会失败，所以直接返回指针而不是 Result
}

}  // namespace tether::client
