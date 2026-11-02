// NSPasteboard 剪贴板读写
//
//   NSPasteboard *pb = [NSPasteboard generalPasteboard];
//   pb.changeCount                                    内容每变一次加一，没有变化通知，只能轮询
//   [pb stringForType:NSPasteboardTypeString]         读文本，没有返回 nil
//   [pb clearContents]; [pb setString:s forType:NSPasteboardTypeString];   写
//   NSString <-> UTF-8：[s UTF8String]、[[NSString alloc] initWithBytes:length:encoding:NSUTF8StringEncoding]
//
// 线程：NSPasteboard 可以在非主线程用
//
// 参考：
//   Apple 文档 "NSPasteboard"

#include "platform/macos/objc_bridge.h"

// #import <AppKit/AppKit.h>

namespace tether::agent::macos_backend::objc {

long pasteboard_change_count() {
    // TODO: return [NSPasteboard generalPasteboard].changeCount;
    return 0;
}

bool pasteboard_get_text(std::string&) {
    // TODO
    return false;
}

void pasteboard_set_text(const std::string&) {
    // TODO
}

}  // namespace tether::agent::macos_backend::objc
