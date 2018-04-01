#include <crtdbg.h>
#include "System\Window.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdLine, int cmdShow)
{
#if _DEBUG
	// 메모리 누수체크 설정 (Debug모드에서만 가능)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Window* window = new Window;
	window->Initialize();
	window->Loop();
	SafeDeletePtr(window);

	return 0;
}