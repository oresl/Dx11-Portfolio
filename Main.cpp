#include "System\Window.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdLine, int cmdShow)
{
	srand((unsigned)time(NULL));

	Window* window = new Window;
	window->Initialize();
	window->Loop();
	SafeDeletePtr(window);

	return 0;
}