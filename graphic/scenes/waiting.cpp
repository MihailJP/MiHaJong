#include "waiting.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

ConnectionWaitingProto::ConnectionWaitingProto(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
}
ConnectionWaitingProto::~ConnectionWaitingProto() {
}

// -------------------------------------------------------------------------

ServerWait::ServerWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
}
ServerWait::~ServerWait() {
}
void ServerWait::Render() {
}

// -------------------------------------------------------------------------

ClientWait::ClientWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
}
ClientWait::~ClientWait() {
}
void ClientWait::Render() {
}

// -------------------------------------------------------------------------

}
