#include "Sphere.h"

// public
Sphere::Sphere()
{
	mVertexData.clear();
	mIndexData.clear();
}

Sphere::~Sphere()
{
	mIndexData.clear();
	mVertexData.clear();
}

void Sphere::Initialize()
{
	mInstanced = false;
	mTessellated = false;
	mInputLayoutSize = sizeof(IL_PosTexNor);

	InitializeData();
	InitializeBuffer();
}

void Sphere::SetBuffer()
{
	SetWorldBuffer();
}

// private
void Sphere::InitializeData()
{
	GetInputLayoutFromFile(L"Sphere");

	mVertexData.resize(mVertexCount);
	mIndexData.resize(mIndexCount);

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVertexData[i] = mTempData[i];
		mIndexData[i] = i;
	}
}

void Sphere::InitializeBuffer()
{
	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(IL_PosTexNor) * mVertexCount,
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