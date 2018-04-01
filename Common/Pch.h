#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

// DirectX 11
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
//#include <D3Dcompiler.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
//#pragma comment(lib, "d3dcompiler.lib")

// 메모리 해제 매크로
#define SafeDeletePtr(ptr)	if(ptr) { delete ptr;		ptr = NULL; }
#define SafeDeleteArr(arr)	if(arr) { delete[] arr;		arr = NULL; }
#define SafeReleaseCom(com) if(com) { com->Release();	com = NULL; }

// Config 구조체 및 글로벌 변수
namespace config
{
	typedef struct CONFIG_GAME_ENGINE
	{
		wstring		Name;
		HINSTANCE	Instance;
		HWND		Hwnd;
		BOOL		Windowed;
		BOOL		VsyncEnabled;
		BOOL		MSAAEnabled;
		UINT		ScreenX;
		UINT		ScreenY;
		FLOAT		ScreenNear;
		FLOAT		ScreenDepth;
		FLOAT		CameraSpeed;
	} CF_ENGINE;

	typedef struct CONFIG_LOG
	{
		UINT TimeStampType;
		UINT Level;
	} CF_LOG;
}

using namespace config;

extern CF_ENGINE	gCFEngine;
extern CF_LOG		gCFLog;

// Custom 헤더
#include "Func.h"

using namespace func_math;
using namespace func_dir;

#include "Singleton.h"
#include "Log.h"

using namespace log_write;

