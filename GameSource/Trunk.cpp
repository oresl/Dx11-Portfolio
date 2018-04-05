#include "Trunk.h"

// public
Trunk::Trunk()
{
	mVertexData.clear();
	mIndexData.clear();
}

Trunk::~Trunk()
{
	mIndexData.clear();
	mVertexData.clear();
}

void Trunk::Initialize(Terrain* terrain, FLOAT scale)
{
	mInstanced					= true;
	mTessellated				= false;
	mInputLayoutSize			= sizeof(IL_PosTex);
	mInputLayoutInstanceSize	= sizeof(IL_PosTexInst);

	InitializeData(terrain, scale);
	InitializeBuffer();
}

void Trunk::Update(Frustum* frustum)
{
	mDrawCount = 0;

	for (UINT i = 0; i < mInstanceCount; i++)
	{
		//if (!frustum->CheckShpere(mInstancePos[i], 1.0F))
		//	continue;

		mDrawCount++;
	}
}

void Trunk::SetBuffer()
{
	// 각 월드를 갖기때문에 x
}

D3DXVECTOR3 Trunk::GetInstancePos(int index) { return mInstancePos[index]; }

// private
void Trunk::InitializeData(Terrain* terrain, FLOAT scale)
{
	// TODO:: Terrain blendmap에서 rgb얻어서 위치를 계산해 나무를 그려보아요..

	// Vertex, Index
	GetInputLayoutFromFile(L"Trunk", scale);

	mVertexData.resize(mVertexCount);
	mIndexData.resize(mIndexCount);

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVertexData[i].Pos = mTempData[i].Pos;
		mVertexData[i].Tex = mTempData[i].Tex;

		mIndexData[i] = i;
	}

	// Instance
	mInstanceCount = gCFTree.Count;
	mInstanceData.resize(mInstanceCount);
	mInstancePos.resize(mInstanceCount);

	for (UINT i = 0; i < mInstanceCount; i++)
	{
		D3DXMATRIX instWorld;
		D3DXMatrixIdentity(&instWorld);

		FLOAT posX;
		FLOAT posZ;
		FLOAT posY;
		
		// height level을 blendmap에서 가져와서 랜덤으로 뿌리는게 좀더 똑똑해보이는 방법같은데..
		do
		{
			posX = (FLOAT)(rand() % ((gCFTerrain.Width - 1) / 2)) - ((gCFTerrain.Width - 1) / 2) * 0.5F;
			posZ = (FLOAT)(rand() % ((gCFTerrain.Depth - 1) / 2)) - ((gCFTerrain.Depth - 1) / 2) * 0.5F;
			posY = terrain->GetHeight(posX, posZ) - 0.5F; // 땅속에 조금 박아버려야지..
		} while (posY < 17.0F || posY > 32.0F);
		

		// 방향랜덤
		D3DXMATRIX rotate, trans;

		D3DXMatrixRotationY(&rotate, rand() * 0.0174532925F);
		D3DXMatrixTranslation(&trans, posX, posY, posZ);
		D3DXMatrixMultiply(&instWorld, &rotate, &trans);

		mInstancePos[i]			= D3DXVECTOR3(posX, posY, posZ);
		mInstanceData[i].World	= instWorld;
	}
}

void Trunk::InitializeBuffer()
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