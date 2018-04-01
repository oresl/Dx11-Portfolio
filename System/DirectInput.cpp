#include "DirectInput.h"

// static
Singleton_cpp(DirectInput)

// public
DirectInput::DirectInput()
{
	mDirectInput	= NULL;
	mKeyboard		= NULL;
	mMouse			= NULL;
}

DirectInput::~DirectInput()
{
	mMouse->Unacquire();
	SafeReleaseCom(mMouse);

	mKeyboard->Unacquire();
	SafeReleaseCom(mKeyboard);

	SafeReleaseCom(mDirectInput);
}

void DirectInput::Initialize()
{
	InitializeDirectInput();
	InitializeKeyboard();
	InitializeMouse();
}

void DirectInput::Update()
{
	UpdateKeyboard();
	UpdateMouse();
}

// 키를 누르면	''	 return
// 키를 때면		'/0' return
bool DirectInput::KeyDown(BYTE key)
{ 
	return mKeyState[key] & 0x80 ? true : false;
}

bool DirectInput::KeyUp(BYTE key)
{ 
	return !(mKeyState[key] & 0x80) && (mKeyStatePrev[key] & 0x80) ? true : false;
}

bool DirectInput::ButtonDown(BYTE button)
{
	return mMouseState.rgbButtons[button] & 0x80 ? true : false;
}

bool DirectInput::ButtonUp(BYTE button)
{
	return !(mMouseState.rgbButtons[button] & 0x80) && (mMouseStatePrev.rgbButtons[button] & 0x80) ? true : false;
}

bool DirectInput::GetMouseStatelX()		{ return mMouseState.lX != mMouseStatePrev.lX; }
bool DirectInput::GetMouseStatelY()		{ return mMouseState.lY != mMouseStatePrev.lY; }
LONG DirectInput::GetPreviousMouselX()	{ return mMouseStatePrev.lX; }
LONG DirectInput::GetCurrentMouselY()	{ return mMouseState.lY; }

// private
void DirectInput::InitializeDirectInput()
{
	HRESULT hr = DirectInput8Create
	(
		gCFEngine.Instance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&mDirectInput,
		NULL
	);
	assert(SUCCEEDED(hr));
}

void DirectInput::InitializeKeyboard()
{
	HRESULT hr = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	assert(SUCCEEDED(hr));

	hr = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = mKeyboard->SetCooperativeLevel(gCFEngine.Hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	// DISCL_BACKGROUND | DISCL_EXCLUSIVE
	assert(SUCCEEDED(hr));

	mKeyboard->Acquire();

	// 키보드상태 초기화
	ZeroMemory(&mKeyState, sizeof(BYTE) * 256);
	ZeroMemory(&mKeyStatePrev, sizeof(BYTE) * 256);
}

void DirectInput::InitializeMouse()
{
	HRESULT hr = mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	assert(SUCCEEDED(hr));

	hr = mMouse->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(hr));

	hr = mMouse->SetCooperativeLevel(gCFEngine.Hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); // DISCL_BACKGROUND | DISCL_EXCLUSIVE
	assert(SUCCEEDED(hr));

	mMouse->Acquire();

	// 마우스상태 초기화
	ZeroMemory(&mMouseState, sizeof(DIMOUSESTATE2));
	ZeroMemory(&mMouseStatePrev, sizeof(DIMOUSESTATE2));

	// 마우스위치 초기화 (모니터 중앙)
	mMousePosition.x = (LONG)(GetSystemMetrics(SM_CXSCREEN) / 2);
	mMousePosition.y = (LONG)(GetSystemMetrics(SM_CYSCREEN) / 2);
	mMousePositionPrev = mMousePosition;

	SetCursorPos((int)mMousePosition.x, (int)mMousePosition.y);
}

void DirectInput::UpdateKeyboard()
{
	memcpy(mKeyStatePrev, mKeyState, sizeof(BYTE) * 256);

	if (FAILED(mKeyboard->GetDeviceState(sizeof(BYTE) * 256, (LPVOID)&mKeyState)))
		mKeyboard->Acquire();
}

void DirectInput::UpdateMouse()
{
	memcpy(&mMouseStatePrev, &mMouseState, sizeof(DIMOUSESTATE2));

	if (FAILED(mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&mMouseState)))
		mMouse->Acquire();

	mMousePositionPrev = mMousePosition;

	mMousePosition.x += mMouseState.lX;
	mMousePosition.y += mMouseState.lY;
}