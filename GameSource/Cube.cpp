#include "Cube.h"

// public
Cube::Cube()
{
	mVertexData.clear();
	mIndexData.clear();
}

Cube::~Cube()
{
	mIndexData.clear();
	mVertexData.clear();
}

void Cube::Initialize()
{
	mInstanced			= false;
	mTessellated		= false;
	mInputLayoutSize	= sizeof(IL_PosTexNor);

	InitializeData();
	InitializeBuffer();
}

void Cube::SetBuffer()
{
	// World
	D3DXMATRIX world = mWorld;
	D3DXMatrixTranspose(&world, &world);

	BufferData data;
	data.World = world;

	SetConstantBuffer(D3D.GetContext(), mWorldBuffer, &data, sizeof(BufferData), 0);
}

// private
void Cube::InitializeData()
{
	GetInputLayoutFromFile(L"Cube");

	mVertexData.resize(mVertexCount);
	mIndexData.resize(mIndexCount);

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVertexData[i]	= mTempData[i];
		mIndexData[i]	= i;
	}
}

void Cube::InitializeBuffer()
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

	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_CONSTANT_BUFFER,
		sizeof(BufferData),
		D3D11_USAGE_DYNAMIC,
		NULL,
		&mWorldBuffer
	);
}