#include "Leaf.h"

// public
Leaf::Leaf()
{
	mVertexData.clear();
	mIndexData.clear();
}

Leaf::~Leaf()
{
	mIndexData.clear();
	mVertexData.clear();
}

void Leaf::Initialize(Trunk* trunk, FLOAT scale)
{
	mInstanced					= true;
	mTessellated				= false;
	mInputLayoutSize			= sizeof(IL_PosTex);
	mInputLayoutInstanceSize	= sizeof(IL_PosTexInst);

	InitializeData(trunk, scale);
	InitializeBuffer();
}

void Leaf::Update(Frustum* frustum)
{
	mDrawCount = 0;

	for (UINT i = 0; i < mInstanceCount; i++)
	{
		//if (!frustum->CheckShpere(mInstancePos[i], 1.0F))
		//	continue;

		mDrawCount++;
	}
}

void Leaf::SetBuffer()
{
	// 각 월드를 갖기때문에 x
}

// private
void Leaf::InitializeData(Trunk* trunk, FLOAT scale)
{
	GetInputLayoutFromFile(L"Leaf", scale);

	mVertexData.resize(mVertexCount);
	mIndexData.resize(mIndexCount);

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVertexData[i].Pos = mTempData[i].Pos;
		mVertexData[i].Tex = mTempData[i].Tex;

		mIndexData[i] = i;
	}

	// Instance
	mInstanceCount = trunk->GetInstanceCount();
	mInstanceData.resize(mInstanceCount);
	mInstancePos.resize(mInstanceCount);

	for (UINT i = 0; i < mInstanceCount; i++)
	{
		D3DXMATRIX instWorld;
		D3DXMatrixIdentity(&instWorld);

		D3DXVECTOR3 pos = trunk->GetInstancePos(i);
		
		// 방향랜덤
		D3DXMATRIX rotate, trans;

		D3DXMatrixRotationY(&rotate, rand() * 0.0174532925F);
		D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&instWorld, &rotate, &trans);

		mInstancePos[i]			= pos;
		mInstanceData[i].World	= instWorld;
	}
}

void Leaf::InitializeBuffer()
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
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(IL_PosInst) * mInstanceCount,
		D3D11_USAGE_IMMUTABLE,
		&mInstanceData[0],
		&mInstanceBuffer
	);
}