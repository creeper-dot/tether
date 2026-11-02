#include "platform/linux/portal_session.h"

// #include <gio/gio.h>

namespace tether::agent::linux_backend {

PortalSession::PortalSession() = default;
PortalSession::~PortalSession() = default;

Status PortalSession::start(const std::string&) {
    // TODO
    return fail(Errc::Unsupported);
}

void PortalSession::notify_pointer_motion_absolute(double, double) {}
void PortalSession::notify_pointer_button(int, bool) {}
void PortalSession::notify_pointer_axis(double, double) {}
void PortalSession::notify_keyboard_keycode(int, bool) {}

}  // namespace tether::agent::linux_backend
