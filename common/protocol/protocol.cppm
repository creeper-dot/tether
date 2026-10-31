// tether.protocol  协议消息定义
// 参考：
//   tigervnc/common/rfb/msgTypes.h                        消息类型编号怎么分配
//   tigervnc/common/rfb/CMsgWriter.cxx / SMsgWriter.cxx   每种消息怎么写
//   tigervnc/common/rfb/CMsgReader.cxx / SMsgReader.cxx   每种消息怎么读、怎么校验
//   tigervnc/common/rfb/ScreenSet.h                       多显示器布局的数据结构
//   FreeRDP/channels/cliprdr、disp                         RDP 怎么设计剪贴板和改分辨率消息
//
// 每种消息：一个 struct + 一对 encode()/decode_xxx()。
//   encode：struct -> Message（type + payload），不会失败
//   decode：Message -> Result<struct>，type 不对或格式不对返回 Errc::Malformed
//
// 改协议格式时把 kProtocolVersion 加 1。Hello 里双方交换版本，不一致就断开。
//
// 方向标记：A->C 表示 agent 发给 client，C->A 反之，<-> 双向。
// 实现顺序：先做标了 [MVP] 的，其余做到对应功能再实现。
//
// import tether.protocol; 会顺带导入 tether.io 和 tether.util。
// 测试：tests/test_messages.cpp

module;

#include <cstdint>
#include <string>
#include <vector>

export module tether.protocol;

export import tether.io;

export namespace tether::proto {

inline constexpr std::uint32_t kProtocolVersion = 1;

// 编号分段：0x0_ 控制  0x1_ 画面  0x2_ 输入  0x3_ 光标/剪贴板  0x4_ 音频
enum MsgType : std::uint8_t {
    // ---- 控制 ----
    kHello = 0x01,            // <->  [MVP] 握手
    kPing = 0x02,             // <->  [MVP] 测 RTT
    kPong = 0x03,             // <->  [MVP]
    kStreamConfig = 0x04,     // C->A [MVP] client 想要的编码、码率、帧率
    kUdpOffer = 0x05,         // A->C       QUIC 端口和凭据（只通过 SSH 通道发）
    kStats = 0x06,            // C->A       client 侧统计：解码耗时、丢帧数，用于调码率
    kBye = 0x07,              // <->        正常断开，带原因

    // ---- 画面 ----
    kVideoFrame = 0x10,       // A->C [MVP]
    kFrameAck = 0x11,         // C->A [MVP] 流控
    kRequestKeyframe = 0x12,  // C->A       解码出错时请求关键帧
    kDisplayLayout = 0x13,    // A->C       显示器数量、各自位置和尺寸
    kResizeRequest = 0x14,    // C->A       client 窗口变了，请求远端改分辨率
    kSelectDisplay = 0x15,    // C->A       多显示器时选看哪一个

    // ---- 输入 ----
    kMouseMove = 0x20,        // C->A [MVP] 绝对坐标
    kMouseRelMove = 0x21,     // C->A       相对移动（游戏、3D 软件锁定鼠标时）
    kMouseButton = 0x22,      // C->A [MVP]
    kMouseScroll = 0x23,      // C->A       滚轮，支持触控板的细粒度滚动
    kKey = 0x24,              // C->A [MVP]
    kReleaseAllKeys = 0x25,   // C->A       client 窗口失去焦点时发，防止远端按键卡住

    // ---- 光标 / 剪贴板 ----
    kCursorShape = 0x30,      // A->C       光标图像
    kCursorPos = 0x31,        // A->C       远端光标位置（远端程序自己移动鼠标时需要）
    kClipboard = 0x32,        // <->        剪贴板文本

    // ---- 音频 ----
    kAudioConfig = 0x40,      // A->C       采样率、声道数
    kAudioPacket = 0x41,      // A->C       一个 Opus 包
};

// ============================== 控制 ==============================

enum Capability : std::uint32_t {
    kCapH264 = 1u << 0,
    kCapHevc = 1u << 1,
    kCapAudio = 1u << 2,
    kCapQuic = 1u << 3,
    kCapClipboard = 1u << 4,
    kCapResize = 1u << 5,
};

struct Hello {
    std::uint32_t version = kProtocolVersion;
    std::string name;               // "tether-agent 0.1"，方便调试
    std::uint32_t capabilities = 0; // Capability 位掩码
};

struct Ping {                       // Pong 用同一个结构，原样回传
    std::uint64_t timestamp_us = 0;
};

enum class Codec : std::uint8_t {
    Raw = 0,                        // 未压缩 BGRA，MVP
    H264 = 1,
    Hevc = 2,
};

struct StreamConfig {
    Codec codec = Codec::Raw;
    std::uint32_t bitrate_kbps = 8000;
    std::uint16_t max_fps = 60;
    bool audio = false;
};

struct UdpOffer {
    std::uint16_t port = 0;
    std::vector<std::uint8_t> cert_sha256;   // agent 自签证书的指纹，client 用它校验
    std::vector<std::uint8_t> token;         // 一次性令牌，client 连上 QUIC 后第一条消息带上
};

struct Stats {
    std::uint32_t frames_received = 0;
    std::uint32_t frames_dropped = 0;        // client 解码失败或来不及显示
    std::uint32_t avg_decode_us = 0;
    std::uint32_t rtt_us = 0;
};

struct Bye {
    std::string reason;
};

// ============================== 画面 ==============================

struct VideoFrame {
    std::uint64_t frame_id = 0;
    std::uint64_t capture_time_us = 0;       // agent 截屏时刻（agent 自己的时钟）
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    Codec codec = Codec::Raw;
    bool keyframe = true;
    std::vector<std::uint8_t> data;
};

struct FrameAck {
    std::uint64_t frame_id = 0;
};

struct RequestKeyframe {};

struct Screen {
    std::uint32_t id = 0;
    std::int32_t x = 0, y = 0;               // 在整个虚拟桌面里的位置
    std::uint16_t width = 0, height = 0;
    bool primary = false;
};

struct DisplayLayout {
    std::uint16_t desktop_width = 0;         // 整个虚拟桌面大小
    std::uint16_t desktop_height = 0;
    std::vector<Screen> screens;
};

struct ResizeRequest {
    std::uint16_t width = 0;
    std::uint16_t height = 0;
};

inline constexpr std::uint32_t kAllScreens = 0xFFFFFFFF;

struct SelectDisplay {
    std::uint32_t screen_id = kAllScreens;
};

// ============================== 输入 ==============================

struct MouseMove {
    std::int32_t x = 0;                      // 远端坐标（client 负责从窗口坐标换算）
    std::int32_t y = 0;
};

struct MouseRelMove {
    std::int16_t dx = 0;
    std::int16_t dy = 0;
};

struct MouseButton {
    std::uint8_t button = 0;                 // 1 左 2 中 3 右 8 后退 9 前进（和 X11 一致）
    bool down = false;
};

struct MouseScroll {
    std::int16_t dx = 0;                     // 单位：1/120 格（和 Windows WHEEL_DELTA 一致）
    std::int16_t dy = 0;                     //       普通鼠标一格 = 120，触控板可以更小
};

struct Key {
    std::uint32_t keysym = 0;                // X11 keysym，表示"这个键代表什么字符/功能"
    std::uint32_t scancode = 0;              // 物理位置码（evdev 编码），游戏和快捷键更可靠
    bool down = false;
};

struct ReleaseAllKeys {};

// ========================== 光标 / 剪贴板 ==========================

struct CursorShape {
    std::uint16_t width = 0, height = 0;
    std::uint16_t hotspot_x = 0, hotspot_y = 0;
    std::vector<std::uint8_t> rgba;          // width*height*4，预乘 alpha
};

struct CursorPos {
    std::int32_t x = 0, y = 0;
    bool visible = true;
};

struct Clipboard {
    std::string text;                        // UTF-8；以后可扩展 mime 类型
};

// ============================== 音频 ==============================

struct AudioConfig {
    std::uint32_t sample_rate = 48000;
    std::uint8_t channels = 2;
};

struct AudioPacket {
    std::uint64_t timestamp_us = 0;
    std::vector<std::uint8_t> opus;
};

// ============================ 编解码函数 ============================

Message encode(const Hello& m);
Message encode(const Ping& m);          // type = kPing；回 Pong 时改 msg.type 即可
Message encode(const StreamConfig& m);
Message encode(const UdpOffer& m);
Message encode(const Stats& m);
Message encode(const Bye& m);
Message encode(const VideoFrame& m);
Message encode(const FrameAck& m);
Message encode(const RequestKeyframe& m);
Message encode(const DisplayLayout& m);
Message encode(const ResizeRequest& m);
Message encode(const SelectDisplay& m);
Message encode(const MouseMove& m);
Message encode(const MouseRelMove& m);
Message encode(const MouseButton& m);
Message encode(const MouseScroll& m);
Message encode(const Key& m);
Message encode(const ReleaseAllKeys& m);
Message encode(const CursorShape& m);
Message encode(const CursorPos& m);
Message encode(const Clipboard& m);
Message encode(const AudioConfig& m);
Message encode(const AudioPacket& m);

Result<Hello> decode_hello(const Message& msg);
Result<Ping> decode_ping(const Message& msg);   // kPing 和 kPong 都能解
Result<StreamConfig> decode_stream_config(const Message& msg);
Result<UdpOffer> decode_udp_offer(const Message& msg);
Result<Stats> decode_stats(const Message& msg);
Result<Bye> decode_bye(const Message& msg);
Result<VideoFrame> decode_video_frame(const Message& msg);
Result<FrameAck> decode_frame_ack(const Message& msg);
Result<DisplayLayout> decode_display_layout(const Message& msg);
Result<ResizeRequest> decode_resize_request(const Message& msg);
Result<SelectDisplay> decode_select_display(const Message& msg);
Result<MouseMove> decode_mouse_move(const Message& msg);
Result<MouseRelMove> decode_mouse_rel_move(const Message& msg);
Result<MouseButton> decode_mouse_button(const Message& msg);
Result<MouseScroll> decode_mouse_scroll(const Message& msg);
Result<Key> decode_key(const Message& msg);
Result<CursorShape> decode_cursor_shape(const Message& msg);
Result<CursorPos> decode_cursor_pos(const Message& msg);
Result<Clipboard> decode_clipboard(const Message& msg);
Result<AudioConfig> decode_audio_config(const Message& msg);
Result<AudioPacket> decode_audio_packet(const Message& msg);
// RequestKeyframe / ReleaseAllKeys 没有内容，看 type 就够了，不需要 decode

}  // namespace tether::proto
