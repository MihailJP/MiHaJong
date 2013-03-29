#include "setting.h"

namespace mihajong_graphic {

PreferenceConfigScene::PreferenceConfigScene(ScreenManipulator* const manipulator) : ConfigMenuProto(manipulator) {
	CreateButton(0, 1240, 1000, 156, 48, _T("‚n ‚j"));
	CreateButton(1, 1060, 1000, 156, 48, _T("CANCEL"));
	redrawItems();
}

PreferenceConfigScene::~PreferenceConfigScene() {
}

void PreferenceConfigScene::redrawItems() {
}
void PreferenceConfigScene::ShowPageCaption() {
}
void PreferenceConfigScene::ShowMessageBelow() {
}

void PreferenceConfigScene::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
}
void PreferenceConfigScene::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
}

}
