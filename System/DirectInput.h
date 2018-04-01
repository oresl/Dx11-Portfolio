#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dInput.h>
#include "..\Common\Pch.h"

#define DIM_LBUTTON 0
#define DIM_MBUTTON 1
#define DIM_RBUTTON 2

class DirectInput
{
public:
	DirectInput();
	~DirectInput();
	void					Initialize();
	void					Update();

	// Get Set
	bool					KeyDown(BYTE key);
	bool					KeyUp(BYTE key);
	bool					ButtonDown(BYTE button);
	bool					ButtonUp(BYTE button);

	bool					GetMouseStatelX();
	bool					GetMouseStatelY();
	LONG					GetPreviousMouselX();
	LONG					GetCurrentMouselY();

	Singleton_h(DirectInput)

private:
	IDirectInput8*			mDirectInput;
	IDirectInputDevice8*	mKeyboard;
	IDirectInputDevice8*	mMouse;
	BYTE					mKeyState[256];
	BYTE					mKeyStatePrev[256];
	DIMOUSESTATE2			mMouseState;
	DIMOUSESTATE2			mMouseStatePrev;
	POINT					mMousePosition;
	POINT					mMousePositionPrev;

	void					InitializeDirectInput();
	void					InitializeKeyboard();
	void					InitializeMouse();

	void					UpdateKeyboard();
	void					UpdateMouse();
};
