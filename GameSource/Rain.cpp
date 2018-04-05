#include "Rain.h"

// public
Rain::Rain()
{
	mVertexData.clear();
	mIndexData.clear();
}

Rain::~Rain()
{
	mVertexData.clear();
	mIndexData.clear();
}

void Rain::Initialize()
{
	mInstanced					= true;
	mTessellated				= false;
	mInputLayoutSize			= sizeof(IL_Pos);
	mInputLayoutInstanceSize	= sizeof(IL_PosInst);

	InitializeData();
	InitializeBuffer();
}

void Rain::Update(FLOAT delta, Camera* cam, Frustum* frustum)
{
	mDrawCount = 0;

	D3DXVECTOR3 camPos = cam->GetPosition();

	vector<IL_PosInst> data;
	data.resize(mInstanceCount);

	for (UINT i = 0; i < mInstanceCount; i++)
	{
		if (!frustum->CheckShpere(mInstancePos[i], 1.0F))
			continue;

		// �ּ���ġ�� �ٴٸ��� ������ y��ġ�� �̵�
		if (mInstancePos[i].y <= gCFRain.EndPosY)
			mInstancePos[i].y = (FLOAT)rand() / (FLOAT)RAND_MAX * gCFRain.StartPosY;

		// �ν��Ͻ��� ��ġ����
		D3DXVECTOR3 instPos;

		instPos.x = mInstancePos[i].x;
		instPos.y = mInstancePos[i].y - (delta * gCFRain.Speed);
		instPos.z = mInstancePos[i].z;

		// ������ ���
		FLOAT angle = atan2(instPos.x - camPos.x, instPos.z - camPos.z) * (180.0F / (FLOAT)D3DX_PI);

		// ���� ������Ʈ
		D3DXMATRIX rotate, trans;

		D3DXMatrixRotationY(&rotate, angle * 0.0174532925F);
		D3DXMatrixTranslation(&trans, instPos.x, instPos.y, instPos.z);
		D3DXMatrixMultiply(&mInstanceData[i].World, &rotate, &trans);

		// ����� ��������
		mInstancePos[i] = instPos;

		// �׸� �����͸� ������Ʈ
		data[mDrawCount] = mInstanceData[i];
		mDrawCount++;
	}

	UpdateVertexBuffer(D3D.GetContext(), mInstanceBuffer, &data[0], sizeof(IL_PosInst) * mInstanceCount);
}

void Rain::SetBuffer()
{
	// �� ���带 ���⶧���� x
}

// private
void Rain::InitializeData()
{
	// �簢��
	mVertexCount = 6;

	// Vertex
	mVertexData.resize(mVertexCount);

	// BL > TR > BR
	mVertexData[0].Pos = D3DXVECTOR3(-0.01F, -0.2F, 0.0F);
	mVertexData[1].Pos = D3DXVECTOR3( 0.01F,  0.2F, 0.0F);
	mVertexData[2].Pos = D3DXVECTOR3( 0.01F, -0.2F, 0.0F);

	// TR > BL > TL
	mVertexData[3].Pos = D3DXVECTOR3( 0.01F,  0.2F, 0.0F);
	mVertexData[4].Pos = D3DXVECTOR3(-0.01F, -0.2F, 0.0F);
	mVertexData[5].Pos = D3DXVECTOR3(-0.01F,  0.2F, 0.0F);

	// Index
	mIndexCount = 6;

	mIndexData.resize(mIndexCount);
	
	for (UINT i = 0; i < mIndexCount; i++)
	{
		mIndexData[i] = i;
	}

	// Instance
	mInstanceCount = gCFRain.Count;

	mInstanceData.resize(mInstanceCount);
	mInstancePos.resize(mInstanceCount);

	for (UINT i = 0; i < mInstanceCount; i++)
	{
		D3DXMATRIX instWorld;
		D3DXMatrixIdentity(&instWorld);

		// Terrain ������ �� ������
		FLOAT posX = (FLOAT)(rand() % ((gCFTerrain.Width - 1) / 2)) - ((gCFTerrain.Width - 1) / 2) * 0.5F;
		FLOAT posY = (FLOAT)rand() / (FLOAT)RAND_MAX * gCFRain.StartPosY;
		FLOAT posZ = (FLOAT)(rand() % ((gCFTerrain.Depth - 1) / 2)) - ((gCFTerrain.Depth - 1) / 2) * 0.5F;

		// ����
		//instWorld._41 = positionX;
		//instWorld._42 = positionY;
		//instWorld._43 = positionZ;
		D3DXMatrixTranslation(&instWorld, posX, posY, posZ);

		mInstancePos[i].x = posX; //instWorld._41;
		mInstancePos[i].y = posY; //instWorld._42;
		mInstancePos[i].z = posZ; //instWorld._43;

		mInstanceData[i].World = instWorld;
	}

	mDrawCount = 0;
}

void Rain::InitializeBuffer()
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

	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(IL_PosInst) * mInstanceCount,
		D3D11_USAGE_DYNAMIC,
		&mInstanceData[0],
		&mInstanceBuffer
	);
}