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
	} CF_Engine;

	typedef struct CONFIG_LOG
	{
		UINT TimeStampType;
		UINT Level;
	} CF_Log;

	typedef struct CONFIG_TEXTURE
	{
		UINT MipmapMinLevel;
		BOOL CompressedFormat;
		UINT Anisotropy;
	} CF_Texture;

	typedef struct CONFIG_SKYPLANE
	{
		FLOAT	Width;
		FLOAT	Top;
		FLOAT	Bottom;
		UINT	TextureRepeat;
		UINT	Resolution;
		FLOAT	Scale;
		FLOAT	Brightness;
	} CF_SkyPlane;
}

using namespace config;

extern CF_Engine	gCFEngine;
extern CF_Log		gCFLog;
extern CF_Texture	gCFTexture;
extern CF_SkyPlane	gCFSkyPlane;

// Custom 헤더
#include "Func.h"

using namespace func_math;
using namespace func_dir;
using namespace func_shader;

#include "Singleton.h"
#include "Log.h"

using namespace log_write;

// Shader registed Buffer num
namespace shader_buffer
{
	typedef enum REGISTED_BUFFER_ENUM
	{
		RB_World = 0,
		RB_Camera,
		RB_Light,
		RB_Material0,
		RB_Material1,
		RB_Custom = 11,
		REGISTED_BUFFER_COUNT,
	} RB_Type;
}

// 마테리얼 버퍼사용을 위해서
using namespace shader_buffer;

