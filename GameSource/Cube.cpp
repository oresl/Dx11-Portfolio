#include "Cube.h"

// public
Cube::Cube()
{
	mVertexCount	= 0;
	mIndexCount		= 0;
	mVertexData.clear();
	mIndexData.clear();
	mVertexBuffer	= NULL;
	mIndexBuffer	= NULL;
}

Cube::~Cube()
{
	SafeReleaseCom(mIndexBuffer);
	SafeReleaseCom(mVertexBuffer);
	mIndexData.clear();
	mVertexData.clear();
}

void Cube::Initialize()
{
	InitializeData();
	InitializeBuffer();
}

// private
void Cube::InitializeData()
{
	CreateMeshDataFromFile(L"Cube");

	mVertexCount = mIndexCount = mTempData.size();
	mVertexData.resize(mVertexCount);

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVertexData[i] = mTempData[i];
		mIndexData.push_back(i);
	}
}

void Cube::InitializeBuffer()
{
	// Vertex
	CreateBuffer
	(
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(VL_PosTexNor) * mVertexCount,
		D3D11_USAGE_IMMUTABLE,
		&mVertexData[0],
		&mVertexBuffer
	);

	// Index
	CreateBuffer
	(
		D3D11_BIND_INDEX_BUFFER,
		sizeof(UINT) * mIndexCount,
		D3D11_USAGE_IMMUTABLE,
		&mIndexData[0],
		&mIndexBuffer
	);
}