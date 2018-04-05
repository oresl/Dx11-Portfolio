#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\Camera.h"
#include "..\RenderResource\Frustum.h"
#include "..\RenderResource\InputLayout.h"

class Terrain : public InputLayout
{
public:
	Terrain();
	~Terrain();
	void						Initialize();
	void						SetBuffer();
	ID3D11ShaderResourceView*	GetHeightmap();
	FLOAT						GetHeight(FLOAT x, FLOAT z);

private:
	struct BufferData
	{
		// HS
		FLOAT		MinCamDist;
		FLOAT		MaxCamDist;
		FLOAT		MinTessStep;
		FLOAT		MaxTessStep;

		// DS
		D3DXVECTOR4 ClipPlane;

		// PS
		FLOAT		TexelU;
		FLOAT		TexelV;
		FLOAT		CellSpacing;
		FLOAT		Pad;
	};

	vector<IL_PosTex>			mVertexData;
	vector<UINT>				mIndexData;
	vector<FLOAT>				mHeightmapData;

	ID3D11Buffer*				mBuffer;
	ID3D11ShaderResourceView*	mHeightmap;

	virtual void				InitializeData();
	virtual void				InitializeBuffer();
	FLOAT						GetWidth();
	FLOAT						GetDepth();

};