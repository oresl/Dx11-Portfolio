#include "SkySphere.h"

// public
SkySphere::SkySphere()
{
	mVertexData.clear();
	mIndexData.clear();
}

SkySphere::~SkySphere()
{
	mIndexData.clear();
	mVertexData.clear();
}

void SkySphere::Initialize()
{
	mInstanced			= false;
	mTessellated		= false;
	mInputLayoutSize	= sizeof(IL_Pos);

	InitializeData();
	InitializeBuffer();
}

void SkySphere::Update(FLOAT delta)
{
}

void SkySphere::SetBuffer()
{
	SetWorldBuffer();
}

// private
void SkySphere::InitializeData()
{
	GetInputLayoutFromFile(L"Sphere");

	mVertexData.resize(mVertexCount);
	mIndexData.resize(mIndexCount);

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVertexData[i].Pos = mTempData[i].Pos;
		mIndexData[i] = i;
	}
}

void SkySphere::InitializeBuffer()
{
	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(IL_Pos) * mVertexCount,
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
}