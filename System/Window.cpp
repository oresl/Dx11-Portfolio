#include "Window.h"

// static
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ((message == WM_DESTROY) || (message == WM_CLOSE))
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

// public
CF_ENGINE	gCFEngine;
CF_LOG		gCFLog;

Window::Window()
{
	//mGame = NULL;
}

Window::~Window()
{
	//SafeDeletePtr(mGame);
}

void Window::Initialize()
{
	InitializeConfig();
	InitializeWindow();
	InitializeGraphic();
	//mGame = new Game;
}

void Window::Loop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			D3D.Clear();

			D3D.Present();
		}
	}
}

// private
void Window::InitializeConfig()
{
	Config* config = new Config;
	config->Initialize(L"Config\\Config.ini");
	SafeDeletePtr(config);
}

void Window::InitializeWindow()
{
#pragma region 윈도우 클래스EX 등록
	gCFEngine.Instance = GetModuleHandle(NULL);

	WNDCLASSEX wndClassEx;
	ZeroMemory(&wndClassEx, sizeof(WNDCLASSEX));
	wndClassEx.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClassEx.lpfnWndProc		= WndProc;
	wndClassEx.cbClsExtra		= 0;
	wndClassEx.cbWndExtra		= 0;
	wndClassEx.hInstance		= gCFEngine.Instance;
	wndClassEx.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wndClassEx.hIconSm			= wndClassEx.hIcon;
	wndClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClassEx.lpszMenuName		= NULL;
	wndClassEx.lpszClassName	= gCFEngine.Name.c_str();
	wndClassEx.cbSize			= sizeof(WNDCLASSEX);

	ATOM result = RegisterClassEx(&wndClassEx);
	assert(result != 0);
#pragma endregion

#pragma region 윈도우 생성
	int positionX, positionY;

	if (!gCFEngine.Windowed)
	{
		// 전체화면일 경우, 추가설정
		DEVMODE screenDesc;
		ZeroMemory(&screenDesc, sizeof(DEVMODE));
		screenDesc.dmSize		= sizeof(DEVMODE);
		screenDesc.dmPelsWidth	= GetSystemMetrics(SM_CXSCREEN);
		screenDesc.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
		screenDesc.dmBitsPerPel = 32;
		screenDesc.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&screenDesc, CDS_FULLSCREEN);

		// 현재 해상도를 다시 저장
		gCFEngine.ScreenX = screenDesc.dmPelsWidth;
		gCFEngine.ScreenY = screenDesc.dmPelsHeight;

		positionX = 0;
		positionY = 0;
	}
	else
	{
		// 생성된 윈도우를 화면의 중앙에 위치
		positionX = (GetSystemMetrics(SM_CXSCREEN) - gCFEngine.ScreenX) / 2;
		positionY = (GetSystemMetrics(SM_CYSCREEN) - gCFEngine.ScreenY) / 2;
	}

	// OS마다 윈도우의 크기가 다름. 따라서 내부영역을 설정해상도로 맞춤
	RECT rc = { 0, 0, (LONG)gCFEngine.ScreenX, (LONG)gCFEngine.ScreenY };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 엔진설정 값중 윈도우 핸들값에 입력
	gCFEngine.Hwnd = CreateWindowEx
	(
		WS_EX_APPWINDOW,								// 윈도우 스타일
		gCFEngine.Name.c_str(),							// 윈도우 클래스 이름 
		gCFEngine.Name.c_str(),							// 윈도우 이름
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,	// 윈도우 스타일
		positionX, positionY,							// 윈도우의 위치
		rc.right - rc.left, rc.bottom - rc.top,			// 윈도우의 크기
		NULL,											// 부모 윈도우 핸들값 x
		NULL,											// 메뉴바 x
		gCFEngine.Instance,
		NULL
	);
	assert(gCFEngine.Hwnd);

	ShowWindow(gCFEngine.Hwnd, SW_SHOW);
	SetForegroundWindow(gCFEngine.Hwnd);
	SetFocus(gCFEngine.Hwnd);
#pragma endregion
}

void Window::InitializeGraphic()
{
	D3D.Initialize();
}