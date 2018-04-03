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
CF_Engine	gCFEngine;
CF_Log		gCFLog;
CF_Texture	gCFTexture;
CF_SkyPlane gCFSkyPlane;

Window::Window()
{
	mGame	= NULL;
	mTimer	= NULL;
}

Window::~Window()
{
	SafeDeletePtr(mTimer);
	SafeDeletePtr(mGame);
}

void Window::Initialize()
{
	InitializeConfig();
	InitializeWindow();
	InitializeDevices();

	mTimer = new Timer;
	mTimer->Initialize();
	mTimer->SetStart();

	mGame = new Game;
	mGame->Initialize();
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
			mTimer->Update();
			Input.Update();
			mGame->Update(mTimer->GetDeltaTime());

			mGame->PreRender();
			
			D3D.Clear();
			mGame->Render();
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
#pragma region ������ Ŭ����EX ���
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

#pragma region ������ ����
	int positionX, positionY;

	if (!gCFEngine.Windowed)
	{
		// ��üȭ���� ���, �߰�����
		DEVMODE screenDesc;
		ZeroMemory(&screenDesc, sizeof(DEVMODE));
		screenDesc.dmSize		= sizeof(DEVMODE);
		screenDesc.dmPelsWidth	= GetSystemMetrics(SM_CXSCREEN);
		screenDesc.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
		screenDesc.dmBitsPerPel = 32;
		screenDesc.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&screenDesc, CDS_FULLSCREEN);

		// ���� �ػ󵵸� �ٽ� ����
		gCFEngine.ScreenX = screenDesc.dmPelsWidth;
		gCFEngine.ScreenY = screenDesc.dmPelsHeight;

		positionX = 0;
		positionY = 0;
	}
	else
	{
		// ������ �����츦 ȭ���� �߾ӿ� ��ġ
		positionX = (GetSystemMetrics(SM_CXSCREEN) - gCFEngine.ScreenX) / 2;
		positionY = (GetSystemMetrics(SM_CYSCREEN) - gCFEngine.ScreenY) / 2;
	}

	// OS���� �������� ũ�Ⱑ �ٸ�. ���� ���ο����� �����ػ󵵷� ����
	RECT rc = { 0, 0, (LONG)gCFEngine.ScreenX, (LONG)gCFEngine.ScreenY };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// �������� ���� ������ �ڵ鰪�� �Է�
	gCFEngine.Hwnd = CreateWindowEx
	(
		WS_EX_APPWINDOW,								// ������ ��Ÿ��
		gCFEngine.Name.c_str(),							// ������ Ŭ���� �̸� 
		gCFEngine.Name.c_str(),							// ������ �̸�
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,	// ������ ��Ÿ��
		positionX, positionY,							// �������� ��ġ
		rc.right - rc.left, rc.bottom - rc.top,			// �������� ũ��
		NULL,											// �θ� ������ �ڵ鰪 x
		NULL,											// �޴��� x
		gCFEngine.Instance,
		NULL
	);
	assert(gCFEngine.Hwnd);

	ShowWindow(gCFEngine.Hwnd, SW_SHOW);
	SetForegroundWindow(gCFEngine.Hwnd);
	SetFocus(gCFEngine.Hwnd);
#pragma endregion
}

void Window::InitializeDevices()
{
	D3D.Initialize();
	Input.Initialize();
}