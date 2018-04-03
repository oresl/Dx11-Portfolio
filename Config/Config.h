#pragma once
#include "..\Common\Pch.h"

class Config
{
public:
	Config();
	~Config();
	void		Initialize(wstring configPath);

private:
	typedef enum CONFIG_SECTION_TYPE_ENUM
	{
		CS_Engine = 0,
		CS_Log,
		CS_Texture,
		CS_Terrain,
		CS_SkySphere,
		CS_SkyPlane,
		CS_Water,
		CONFIG_SECTION_TYPE_COUNT,
	} CS_Type;

	// Section °ª
	const WCHAR ENGINE[7]		= L"Engine";
	const WCHAR LOG[4]			= L"Log";
	const WCHAR TEXTURE[8]		= L"Texture";
	const WCHAR TERRAIN[8]		= L"Terrain";
	const WCHAR SKYSPHERE[10]	= L"SkySphere";
	const WCHAR SKYPLANE[9]		= L"SkyPlane";
	const WCHAR WATER[6]		= L"Water";

	wstring		mConfigPath;

	void		SetConfig(CS_Type type);
	wstring		GetValueString(const WCHAR* section, const WCHAR* key);
	UINT		GetValueUINT(const WCHAR* section, const WCHAR* key);
	FLOAT		GetValueFLOAT(const WCHAR* section, const WCHAR* key);
	BOOL		GetValueBOOL(const WCHAR* section, const WCHAR* key);
};

