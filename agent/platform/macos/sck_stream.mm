// ScreenCaptureKit 截屏 / 录系统声音（macOS 12.3+，声音要 13+）
//
// 这一个文件同时服务 sck_capture.cpp（画面）和 sck_audio_capture.cpp（声音），
// 因为 ScreenCaptureKit 里画面和声音是同一个 SCStream 的两种输出。
// 现在的做法是画面、声音各开一个 stream（简单）；以后可以合并成一个。
//
// stream_start() 步骤：
//   1. [SCShareableContent getShareableContentWithCompletionHandler:]
//      异步回调，用 dispatch_semaphore 等它返回（stream_start 是阻塞函数）
//      第一次调用会弹"屏幕录制"权限框；没权限时 error 不为空
//   2. 在 content.displays 里找 displayID == opt.display_id 的 SCDisplay
//   3. SCContentFilter *filter = [[SCContentFilter alloc] initWithDisplay:display excludingWindows:@[]];
//   4. SCStreamConfiguration *cfg：
//        width / height        = 像素尺寸（display.width 是点，要乘缩放；
//                                或用 CGDisplayModeGetPixelWidth(CGDisplayCopyDisplayMode(id))）
//        pixelFormat           = kCVPixelFormatType_32BGRA
//        minimumFrameInterval  = CMTimeMake(1, fps)
//        showsCursor           = NO（光标单独传，见 mac_cursor_monitor）
//        queueDepth            = 3 左右，太小会丢帧，太大延迟高
//        capturesAudio         = opt.audio（macOS 13+），sampleRate / channelCount
//        excludesCurrentProcessAudio = YES
//   5. 定义一个实现 SCStreamOutput 协议的 delegate 类（@interface TetherStreamOutput : NSObject <SCStreamOutput>）
//      stream:didOutputSampleBuffer:ofType: 里：
//        SCStreamOutputTypeScreen：
//          先检查 attachments 里的 SCStreamFrameInfoStatus，只有 SCFrameStatusComplete 才是新画面
//          （Idle 表示画面没变，可以直接忽略）
//          CVImageBufferRef pb = CMSampleBufferGetImageBuffer(sb);
//          CVPixelBufferLockBaseAddress(pb, kCVPixelBufferLock_ReadOnly) -> 填 VideoFrameView -> 调 on_video
//          -> CVPixelBufferUnlockBaseAddress
//        SCStreamOutputTypeAudio：
//          CMSampleBufferGetAudioBufferListWithRetainedBlockBuffer 拿 AudioBufferList
//          注意 ScreenCaptureKit 给的是非交错（planar）float32，每个声道一个 buffer，
//          要自己交错成 LRLRLR 再调 on_audio
//   6. SCStream *stream = [[SCStream alloc] initWithFilter:filter configuration:cfg delegate:nil];
//      [stream addStreamOutput:output type:SCStreamOutputTypeScreen sampleHandlerQueue:queue error:&err];
//      [stream startCaptureWithCompletionHandler:] 同样用信号量等结果
//
// 参考：
//   Apple 示例工程 "Capturing screen content in macOS"（ScreenCaptureKit 官方示例，Swift 写的，思路一样）
//   obs-studio/plugins/mac-capture/mac-sck-video-capture.m、mac-sck-audio-capture.m
//     （C/Objective-C 实现，和我们的结构最接近，推荐看这个）
//   Sunshine/src/platform/macos/av_video.m   Sunshine 用的是旧的 AVCaptureScreenInput（已废弃），
//     但 delegate + dispatch queue 的回调结构一样，可以参考
//   Sunshine/src/platform/macos/av_audio.mm  系统声音用的是 Core Audio Process Tap（macOS 14.2+），
//     另一种录系统声音的办法

#include "platform/macos/objc_bridge.h"

// #import <ScreenCaptureKit/ScreenCaptureKit.h>
// #import <CoreMedia/CoreMedia.h>
// #import <CoreVideo/CoreVideo.h>

// TODO: @interface TetherStreamOutput : NSObject <SCStreamOutput>
//         @property 保存 VideoCallback / AudioCallback（std::function 可以直接放在 Objective-C++ 类的 ivar 里）
//       @end
//       @implementation TetherStreamOutput
//         - (void)stream:(SCStream *)stream didOutputSampleBuffer:(CMSampleBufferRef)sb ofType:(SCStreamOutputType)type
//       @end

namespace tether::agent::macos_backend::objc {

struct ScreenStream {
    // TODO: SCStream *stream; TetherStreamOutput *output; dispatch_queue_t queue;
    //       （ARC 下 C++ 结构体里可以直接放 Objective-C 强引用）
};

ScreenStream* stream_start(const StreamOptions&, VideoCallback, AudioCallback, std::string& error) {
    // TODO: 见文件开头
    error = "ScreenCaptureKit stream not implemented";
    return nullptr;
}

void stream_stop(ScreenStream* s) {
    // TODO: [s->stream stopCaptureWithCompletionHandler:]，等它完成后 delete s
    delete s;
}

}  // namespace tether::agent::macos_backend::objc
