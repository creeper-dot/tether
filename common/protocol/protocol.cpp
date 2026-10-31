// tether.protocol 的实现
module;

#include <cstdint>
#include <string>
#include <vector>

module tether.protocol;

namespace tether::proto {

// TODO: 每个函数用 ByteWriter / ByteReader 实现。
//   - encode 和 decode 的字段顺序必须完全一致
//   - decode 先检查 msg.type，不对就 return fail(Errc::Malformed, "...")
//   - decode 最后 if (auto st = r.finish(); !st) return std::unexpected(st.error());
//   - 带 vector 的字段用 blob()，decode 时给上限。比如 CursorShape 的 rgba 必须正好是 width*height*4
//
// 参数名先省略掉，避免"未使用参数"的警告。实现时补上名字。

Message encode(const Hello&) { return {}; }
Message encode(const Ping&) { return {}; }
Message encode(const StreamConfig&) { return {}; }
Message encode(const UdpOffer&) { return {}; }
Message encode(const Stats&) { return {}; }
Message encode(const Bye&) { return {}; }
Message encode(const VideoFrame&) { return {}; }
Message encode(const FrameAck&) { return {}; }
Message encode(const RequestKeyframe&) { return {}; }
Message encode(const DisplayLayout&) { return {}; }
Message encode(const ResizeRequest&) { return {}; }
Message encode(const SelectDisplay&) { return {}; }
Message encode(const MouseMove&) { return {}; }
Message encode(const MouseRelMove&) { return {}; }
Message encode(const MouseButton&) { return {}; }
Message encode(const MouseScroll&) { return {}; }
Message encode(const Key&) { return {}; }
Message encode(const ReleaseAllKeys&) { return {}; }
Message encode(const CursorShape&) { return {}; }
Message encode(const CursorPos&) { return {}; }
Message encode(const Clipboard&) { return {}; }
Message encode(const AudioConfig&) { return {}; }
Message encode(const AudioPacket&) { return {}; }

Result<Hello> decode_hello(const Message&) { return Hello{}; }
Result<Ping> decode_ping(const Message&) { return Ping{}; }
Result<StreamConfig> decode_stream_config(const Message&) { return StreamConfig{}; }
Result<UdpOffer> decode_udp_offer(const Message&) { return UdpOffer{}; }
Result<Stats> decode_stats(const Message&) { return Stats{}; }
Result<Bye> decode_bye(const Message&) { return Bye{}; }
Result<VideoFrame> decode_video_frame(const Message&) { return VideoFrame{}; }
Result<FrameAck> decode_frame_ack(const Message&) { return FrameAck{}; }
Result<DisplayLayout> decode_display_layout(const Message&) { return DisplayLayout{}; }
Result<ResizeRequest> decode_resize_request(const Message&) { return ResizeRequest{}; }
Result<SelectDisplay> decode_select_display(const Message&) { return SelectDisplay{}; }
Result<MouseMove> decode_mouse_move(const Message&) { return MouseMove{}; }
Result<MouseRelMove> decode_mouse_rel_move(const Message&) { return MouseRelMove{}; }
Result<MouseButton> decode_mouse_button(const Message&) { return MouseButton{}; }
Result<MouseScroll> decode_mouse_scroll(const Message&) { return MouseScroll{}; }
Result<Key> decode_key(const Message&) { return Key{}; }
Result<CursorShape> decode_cursor_shape(const Message&) { return CursorShape{}; }
Result<CursorPos> decode_cursor_pos(const Message&) { return CursorPos{}; }
Result<Clipboard> decode_clipboard(const Message&) { return Clipboard{}; }
Result<AudioConfig> decode_audio_config(const Message&) { return AudioConfig{}; }
Result<AudioPacket> decode_audio_packet(const Message&) { return AudioPacket{}; }

}  // namespace tether::proto
