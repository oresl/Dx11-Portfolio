#include "SkyPlane.h"

// public
SkyPlane::SkyPlane()
{
	mBuffer		= NULL;
	mVertexData.clear();
	mIndexData.clear();
	mTranslation = 0.0F;
}

SkyPlane::~SkyPlane()
{
	SafeReleaseCom(mBuffer);
	mIndexData.clear();
	mVertexData.clear();
}

void SkyPlane::Initialize()
{
	mInstanced			= false;
	mTessellated		= false;
	mInputLayoutSize	= sizeof(IL_PosTex);

	InitializeData();
	InitializeBuffer();
}

void SkyPlane::Update(FLOAT delta)
{
	mTranslation += delta * 0.01F;
	if (mTranslation > 1.0F)
		mTranslation -= 1.0F;
}

void SkyPlane::SetBuffer()
{
	SetWorldBuffer();

	BufferData data;
	data.Translation	= mTranslation;
	data.Scale			= gCFSkyPlane.Scale;
	data.Brightness		= gCFSkyPlane.Brightness;
	data.Pad			= 1.0F;

	SetConstantBuffer(D3D.GetContext(), mBuffer, &data, sizeof(BufferData), (UINT)RB_Custom);
}

// private
void SkyPlane::InitializeData()
{
	FLOAT width		= gCFSkyPlane.Width;
	FLOAT top		= gCFSkyPlane.Top;
	FLOAT bottom	= gCFSkyPlane.Bottom;
	UINT repeat		= gCFSkyPlane.TextureRepeat;
	UINT resolution = gCFSkyPlane.Resolution;

	FLOAT quadSize	= width / (FLOAT)resolution;
	FLOAT radius	= width / 2.0F;
		
	// 높이변화값
	FLOAT constant	= (top - bottom) / (radius * radius);
	FLOAT delta		= (FLOAT)repeat / (FLOAT)resolution;

	// Vertex
	mVertexCount	= (resolution + 1) * (resolution + 1);
	mVertexData.resize(mVertexCount);

	for (UINT z = 0; z < resolution + 1; z++)
	{
		for (UINT x = 0; x < resolution + 1; x++)
		{
			// Pos
			FLOAT posX = (-0.5F * width) + ((FLOAT)x * quadSize);
			FLOAT posZ = (-0.5F * width) + ((FLOAT)z * quadSize);
			FLOAT posY = top - (constant * ((posX * posX) + (posZ * posZ)));

			// Tex
			FLOAT tu = (FLOAT)x * delta;
			FLOAT tv = (FLOAT)z * delta;

			int index				= (resolution + 1) * z + x;
			mVertexData[index].Pos	= D3DXVECTOR3(posX, posY, posZ);
			mVertexData[index].Tex	= D3DXVECTOR2(tu, tv);
		}
	}

	// Index
	mIndexCount = resolution * resolution * 6;

	for (UINT z = 0; z < resolution; z++)
	{
		for (UINT x = 0; x < resolution; x++)
		{
			mIndexData.push_back((resolution + 1) * z + x);
			mIndexData.push_back((resolution + 1) * z + x + 1);
			mIndexData.push_back((resolution + 1) * (z + 1) + x);

			mIndexData.push_back((resolution + 1) * z + x + 1);
			mIndexData.push_back((resolution + 1) * (z + 1) + (x + 1));
			mIndexData.push_back((resolution + 1) * (z + 1) + x);
		}
	}
}

void SkyPlane::InitializeBuffer()
{
	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(IL_PosTex) * mVertexCount,
		D3D11_USAGE_IMMUTABLE,
		&mVertexData[0],
		&mVertexBuffer
	);

	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_INDEX_BUFFER,
		sizeof(UINT) * mIndexCount,
		D3D11_USAGE_IMMUTABLE,
		&mIndexData[0],
		&mIndexBuffer
	);

	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_CONSTANT_BUFFER,
		sizeof(BufferData),
		D3D11_USAGE_DYNAMIC,
		NULL,
		&mBuffer
	);
}