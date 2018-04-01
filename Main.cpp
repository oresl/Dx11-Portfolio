#include <crtdbg.h>
#include "System\Window.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdLine, int cmdShow)
{
#if _DEBUG
	// �޸� ����üũ ���� (Debug��忡���� ����)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Window* window = new Window;
	window->Initialize();
	window->Loop();
	SafeDeletePtr(window);

	return 0;
}