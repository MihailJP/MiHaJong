#include "config.h"
#include "resource.h"
#include <Windows.h>
#include <tchar.h>
#include "controls.h"
#include <memory>
#include "dialog.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	DialogWrapper::ConfigDialog myDialog(hInstance);
	myDialog.show();
	
	return 0;
}
