#include "cursor/cursor_monitor.h"

namespace tether::agent {

CursorMonitor::CursorMonitor(const char*) {
    // TODO: XOpenDisplay，XFixesSelectCursorInput
}

CursorMonitor::~CursorMonitor() = default;

bool CursorMonitor::poll_shape(proto::CursorShape&) {
    // TODO: 处理 XFixesCursorNotify 事件，XFixesGetCursorImage
    return false;
}

bool CursorMonitor::poll_position(proto::CursorPos&) {
    // TODO: XQueryPointer，和上次比较
    return false;
}

}  // namespace tether::agent
