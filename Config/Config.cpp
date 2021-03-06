#include "Config.h"
#define CONFIG_BUFFER_MAX 64

// public
Config::Config()
{
	mConfigPath = L"";
}

Config::~Config()
{
	mConfigPath.clear();
}

void Config::Initialize(wstring configPath)
{
	mConfigPath = configPath;

	for (int type = 0; type < (int)CONFIG_SECTION_TYPE_COUNT; type++)
	{
		SetConfig((CS_Type)type);
	}
}

// private
void Config::SetConfig(CS_Type type)
{
	switch (type)
	{
	case CS_Engine:
	{
		ZeroMemory(&gCFEngine, sizeof(CF_Engine));

		gCFEngine.Name			= GetValueString(ENGINE, L"Name");
		gCFEngine.Windowed		= GetValueBOOL(ENGINE, L"Windowed");
		gCFEngine.VsyncEnabled	= GetValueBOOL(ENGINE, L"VsyncEnabled");
		
		UINT screenX			= GetValueUINT(ENGINE, L"Width");
		UINT screenY			= GetValueUINT(ENGINE, L"Height");
		FLOAT screenNear		= GetValueFLOAT(ENGINE, L"Near");
		FLOAT screenDepth		= GetValueFLOAT(ENGINE, L"Depth");
		FLOAT cameraSpeed		= GetValueFLOAT(ENGINE, L"CameraSpeed");
		FLOAT frustumDepth		= GetValueFLOAT(ENGINE, L"FrustumDepth");

		gCFEngine.Width		= Clamp(screenX, (UINT)500, (UINT)GetSystemMetrics(SM_CXSCREEN));
		gCFEngine.Height		= Clamp(screenY, (UINT)500, (UINT)GetSystemMetrics(SM_CYSCREEN));
		gCFEngine.Near	= Clamp(screenNear, 0.1F, 1.0F);
		gCFEngine.Depth	= Clamp(screenDepth, 100.0F, 1000.0F);
		gCFEngine.CameraSpeed	= Clamp(cameraSpeed, 10.0F, 100.0F);

		break;
	}

	case CS_Log:
	{
		ZeroMemory(&gCFLog, sizeof(CF_Log));

		UINT timestampType		= GetValueUINT(LOG, L"TimeStampType");
		UINT level				= GetValueUINT(LOG, L"Level");

		gCFLog.TimeStampType	= Clamp(timestampType, (UINT)0, (UINT)TIMESTAMP_TYPE_COUNT - 1);
		gCFLog.Level			= Clamp(timestampType, (UINT)0, (UINT)LOG_TYPE_COUNT - 1);

		break;
	}

	case CS_Texture:
	{
		ZeroMemory(&gCFLog, sizeof(CF_Texture));

		gCFTexture.CompressedFormat = GetValueBOOL(TEXTURE, L"CompressedFormat");

		UINT mipmapMinLevel			= GetValueUINT(TEXTURE, L"MipmapMinLevel");
		UINT anisotropy				= GetValueUINT(TEXTURE, L"Anisotropy");

		gCFTexture.MipmapMinLevel	= Clamp(mipmapMinLevel, (UINT)1, (UINT)9);
		gCFTexture.Anisotropy		= Clamp(anisotropy, (UINT)1, (UINT)16);

		break;
	}

	case CS_Terrain:
	{
		UINT width				= GetValueUINT(TERRAIN, L"Width");
		UINT depth				= GetValueUINT(TERRAIN, L"Depth");
		UINT tessValue			= GetValueUINT(TERRAIN, L"TessValue");

		FLOAT minCamDist		= GetValueFLOAT(TERRAIN, L"MinCamDist");
		FLOAT maxCamDist		= GetValueFLOAT(TERRAIN, L"MaxCamDist");
		FLOAT minTessStep		= GetValueFLOAT(TERRAIN, L"MinTessStep");
		FLOAT maxTessStep		= GetValueFLOAT(TERRAIN, L"MaxTessStep");
		FLOAT scale				= GetValueFLOAT(TERRAIN, L"Scale");
		FLOAT cellSpacing		= GetValueFLOAT(TERRAIN, L"CellSpacing");

		gCFTerrain.Heightmap	= GetValueString(TERRAIN, L"Heightmap");
		gCFTerrain.Width		= Clamp(width, (UINT)0, UINT_MAX); 
		gCFTerrain.Depth		= Clamp(depth, (UINT)0, UINT_MAX);
		
		gCFTerrain.TessValue	= Clamp(tessValue, (UINT)0, (UINT)64);
		gCFTerrain.MinCamDist	= Clamp(minCamDist, 0.0F, 100.0F);
		gCFTerrain.MaxCamDist	= Clamp(maxCamDist, minCamDist + 100.0F, 600.0F);
		gCFTerrain.MinTessStep	= Clamp(minTessStep, 0.0F, 1.0F);
		gCFTerrain.MaxTessStep	= Clamp(maxTessStep, minTessStep + 1.0F, 6.0F);
		gCFTerrain.Scale		= Clamp(scale, 1.0F, 1000.0F);
		gCFTerrain.CellSpacing	= Clamp(cellSpacing, 0.1F, 1.0F);

		gCFTerrain.Blendmap		= GetValueString(TERRAIN, L"Blendmap");
		gCFTerrain.Layermap0	= GetValueString(TERRAIN, L"Layermap0");
		gCFTerrain.Layermap1	= GetValueString(TERRAIN, L"Layermap1");
		gCFTerrain.Layermap2	= GetValueString(TERRAIN, L"Layermap2");
		gCFTerrain.Layermap3	= GetValueString(TERRAIN, L"Layermap3");

		break;
	}

	case CS_Rain:
	{
		UINT count			= GetValueUINT(RAIN, L"Count");
		FLOAT speed			= GetValueFLOAT(RAIN, L"Speed");
		FLOAT startPosY		= GetValueFLOAT(RAIN, L"StartPosY");
		FLOAT endPosY		= GetValueFLOAT(RAIN, L"EndPosY");

		gCFRain.Count		= Clamp(count, (UINT)100, (UINT)20000);
		gCFRain.Speed		= Clamp(speed, 5.0F, 100.0F);
		gCFRain.StartPosY	= Clamp(startPosY, 20.0F, 200.0F);
		gCFRain.EndPosY		= Clamp(endPosY, -5.0F, 0.0F);
		break;
	}

	case CS_Tree:
	{
		UINT sort		= GetValueUINT(TREE, L"Sort");
		UINT count		= GetValueUINT(TREE, L"Count");

		gCFTree.Sort	= Clamp(sort, (UINT)1, (UINT)5);
		gCFTree.Count	= Clamp(count, (UINT)10, (UINT)200);
	}
	//case CS_SkySphere: { break; }

	case CS_SkyPlane:
	{
		FLOAT width					= GetValueFLOAT(SKYPLANE, L"Width");
		FLOAT top					= GetValueFLOAT(SKYPLANE, L"Top");
		FLOAT bottom				= GetValueFLOAT(SKYPLANE, L"Bottom");
		UINT textureRepeat			= GetValueUINT(SKYPLANE, L"TextureRepeat");
		UINT resolution				= GetValueUINT(SKYPLANE, L"Resolution");
		FLOAT scale					= GetValueFLOAT(SKYPLANE, L"Scale");
		FLOAT brightness			= GetValueFLOAT(SKYPLANE, L"Brightness");
		FLOAT speed					= GetValueFLOAT(SKYPLANE, L"Speed");

		gCFSkyPlane.Width			= Clamp(width, 1.0F, 100.0F);
		gCFSkyPlane.Top				= Clamp(top, 0.0F, 1.0F);
		gCFSkyPlane.Bottom			= Clamp(bottom, -1.0F, 0.0F);
		gCFSkyPlane.TextureRepeat	= Clamp(textureRepeat, (UINT)2, (UINT)8);
		gCFSkyPlane.Resolution		= Clamp(resolution, (UINT)5, (UINT)100);
		gCFSkyPlane.Scale			= Clamp(scale, 0.1F, 5.0F);
		gCFSkyPlane.Brightness		= Clamp(brightness, 0.1F, 1.0F);
		gCFSkyPlane.Speed			= Clamp(speed, 0.01F, 0.1F);

		break;
	}

	case CS_Water:
	{
		gCFWater;
		break;
	}
	}
}

wstring Config::GetValueString(const WCHAR* section, const WCHAR* key)
{
	WCHAR output[CONFIG_BUFFER_MAX];

	GetPrivateProfileString(section, key, L"", output, ARRAYSIZE(output), mConfigPath.c_str());

	return wstring(output);
}

UINT Config::GetValueUINT(const WCHAR* section, const WCHAR* key)
{
	UINT output = GetPrivateProfileInt(section, key, (UINT)0, mConfigPath.c_str());

	// **음수가 들어오면 UINT_MAX값이 들어옴
	if (output >= UINT_MAX)
		return 0;

	return output;
}

FLOAT Config::GetValueFLOAT(const WCHAR* section, const WCHAR* key)
{
	WCHAR output[CONFIG_BUFFER_MAX];

	GetPrivateProfileString(section, key, L"0.0", output, ARRAYSIZE(output), mConfigPath.c_str());

	return (FLOAT)_wtof(output);
}

BOOL Config::GetValueBOOL(const WCHAR* section, const WCHAR* key)
{
	WCHAR output[CONFIG_BUFFER_MAX];

	GetPrivateProfileString(section, key, L"FALSE", output, ARRAYSIZE(output), mConfigPath.c_str());

	if ((wstring(output).find(L"true")) == 0 || (wstring(output).find(L"TRUE")) == 0)
		return TRUE;

	return FALSE;
}