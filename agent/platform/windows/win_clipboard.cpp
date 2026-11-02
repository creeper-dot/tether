// Windows 剪贴板
//
// 变化检测，两种做法：
//   A. GetClipboardSequenceNumber()：每次剪贴板变化这个数字加一，poll() 里比较一下就行
//      不需要窗口，最简单，先用这个
//   B. AddClipboardFormatListener(hwnd) + WM_CLIPBOARDUPDATE 消息：要建一个隐藏窗口（HWND_MESSAGE）
//      并在某个线程里跑消息循环，更"正规"，但对轮询结构的 Session 来说没必要
//
// 读：
//   OpenClipboard(nullptr) -> GetClipboardData(CF_UNICODETEXT) -> GlobalLock 得到 wchar_t*
//   -> wide_to_utf8 -> GlobalUnlock -> CloseClipboard
//   OpenClipboard 可能失败（别的程序正开着），失败就下次 poll 再试，不要死等
//
// 写：
//   GlobalAlloc(GMEM_MOVEABLE, (len+1)*sizeof(wchar_t)) -> GlobalLock 拷进去 -> GlobalUnlock
//   -> OpenClipboard -> EmptyClipboard -> SetClipboardData(CF_UNICODETEXT, hmem) -> CloseClipboard
//   SetClipboardData 成功后内存归系统所有，不要 GlobalFree；失败才要自己 GlobalFree
//
// 防回环：set() 之后记下 GetClipboardSequenceNumber()，poll() 时序号等于它就跳过
//
// 换行：Windows 文本是 \r\n，Linux 是 \n。协议里统一用 \n，读的时候 \r\n -> \n，写的时候反过来
//
// 参考：
//   tigervnc/win/rfb_win32/Clipboard.cxx   完整的 Windows 剪贴板收发，含换行转换
//   learn.microsoft.com "Using the Clipboard"

#include "platform/windows/backends.h"
#include "platform/windows/win_util.h"

namespace tether::agent::windows_backend {

namespace {

class WinClipboard final : public Clipboard {
public:
    bool poll(std::string&) override {
        // TODO: 比较 GetClipboardSequenceNumber()，变了就读 CF_UNICODETEXT
        return false;
    }

    void set(const std::string&) override {
        // TODO: utf8_to_wide，\n -> \r\n，写入，记下序号
    }

private:
    DWORD last_seq_ = 0;          // 上次看到的序号
    DWORD self_set_seq_ = 0;      // 自己 set() 之后的序号（防回环）
};

}  // namespace

Result<std::unique_ptr<Clipboard>> create_win_clipboard() {
    return std::make_unique<WinClipboard>();
}

}  // namespace tether::agent::windows_backend
