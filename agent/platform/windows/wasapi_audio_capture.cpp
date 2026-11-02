// Windows 声音采集：WASAPI loopback（录"正在播放的声音"）
//
// 初始化 start()：
//   1. CoInitializeEx(nullptr, COINIT_MULTITHREADED)
//      COM 是按线程初始化的：要在调用 read() 的那个线程里初始化，结束时 CoUninitialize。
//      所以 start() 最好也在音频线程里调（Session::audio_loop 里先 start 再循环 read）
//   2. CoCreateInstance(__uuidof(MMDeviceEnumerator)) -> GetDefaultAudioEndpoint(eRender, eConsole)
//      注意是 eRender（播放设备），loopback 就是"录播放设备的输出"
//   3. device->Activate(IAudioClient)
//   4. 格式：要 48000 Hz、2 声道、float32。用 WAVEFORMATEXTENSIBLE 填好，
//      Initialize 时加 AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY，
//      让系统帮你转换采样率和格式，就不用自己重采样
//   5. client->Initialize(AUDCLNT_SHAREMODE_SHARED,
//        AUDCLNT_STREAMFLAGS_LOOPBACK | AUDCLNT_STREAMFLAGS_EVENTCALLBACK | 上面两个,
//        缓冲时长（100ns 单位，10ms = 100000）, 0, &format, nullptr)
//   6. CreateEvent -> client->SetEventHandle(event)；client->GetService(IAudioCaptureClient)
//   7. AvSetMmThreadCharacteristicsW(L"Pro Audio", ...) 提高线程优先级（avrt.lib），可选
//   8. client->Start()
//
// 读 read(out, frames)：
//   内部维护一个缓冲区，不够 frames 个采样帧时：
//     WaitForSingleObject(event, 超时) ->
//     循环 capture->GetNextPacketSize -> GetBuffer(&data, &n, &flags) -> 拷进缓冲 -> ReleaseBuffer(n)
//       flags & AUDCLNT_BUFFERFLAGS_SILENT：这段是静音，data 可能无效，要自己填 0
//   够了就取出 frames 个返回
//
// 坑：
//   - 没有任何程序在播放声音时，loopback 不会产生数据（事件不触发），read() 会一直等。
//     解决：等待超时后自己补一段静音；或者像 Sunshine 一样同时播放一段静音流让设备保持活动
//   - 默认播放设备切换（插耳机）后返回 AUDCLNT_E_DEVICE_INVALIDATED，要重新 start()
//
// 参考：
//   Sunshine/src/platform/windows/audio.cpp
//     make_audio_client()（第 358 行附近）   Initialize 的参数、LOOPBACK 和 AUTOCONVERTPCM
//     default_device()（第 421 行附近）       GetDefaultAudioEndpoint
//     mic_wasapi_t::init()（第 609 行附近）   SetEventHandle、AvSetMmThreadCharacteristics
//     mic_wasapi_t::_fill_buffer()（第 746 行附近）  GetBuffer 循环、SILENT 标志、DEVICE_INVALIDATED
//     co_init_t（第 292 行附近）              用 RAII 管 CoInitializeEx / CoUninitialize
//   learn.microsoft.com "Loopback Recording"、"Capturing a Stream"
//
// 依赖：ole32、avrt（可选）

#include "platform/windows/backends.h"
#include "platform/windows/win_util.h"

// #include <audioclient.h>
// #include <mmdeviceapi.h>
// #include <wrl/client.h>

namespace tether::agent::windows_backend {

namespace {

class WasapiAudioCapture final : public AudioCapture {
public:
    ~WasapiAudioCapture() override { stop(); }

    Status start(std::uint32_t, int) override {
        // TODO: 见文件开头"初始化"
        return fail(Errc::Unsupported, "wasapi capture not implemented");
    }

    Status read(std::vector<float>&, int) override {
        // TODO: 见文件开头"读"
        return fail(Errc::Unsupported, "wasapi capture not implemented");
    }

    void stop() override {
        // TODO: client->Stop()，释放 COM 对象，CloseHandle(event)，CoUninitialize
    }

private:
    // TODO: ComPtr<IAudioClient>、ComPtr<IAudioCaptureClient>、HANDLE event、
    //       std::vector<float> pending_（不够一帧时攒着）、bool com_inited_
};

}  // namespace

Result<std::unique_ptr<AudioCapture>> create_wasapi_audio_capture() {
    return std::make_unique<WasapiAudioCapture>();
}

}  // namespace tether::agent::windows_backend
