#include "InputLayout.h"
#define FLOAT1			DXGI_FORMAT_R32_FLOAT
#define FLOAT2			DXGI_FORMAT_R32G32_FLOAT
#define FLOAT3			DXGI_FORMAT_R32G32B32_FLOAT
#define FLOAT4			DXGI_FORMAT_R32G32B32A32_FLOAT
#define VERTEX_DATA		D3D11_INPUT_PER_VERTEX_DATA
#define INSTANCE_DATA	D3D11_INPUT_PER_INSTANCE_DATA
#define ALIGNED			D3D11_APPEND_ALIGNED_ELEMENT

#pragma region D3D11_INPUT_ELEMENT_DESC 커스터마이징 구조체 정의
using namespace inputlayout;

const D3D11_INPUT_ELEMENT_DESC IL_Pos::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
};
const UINT IL_Pos::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC IL_PosTex::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT2, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT IL_PosTex::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC IL_PosTexNor::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT2, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "NORMAL"	,	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT IL_PosTexNor::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC IL_PosTexNorTanBiN::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT2, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "NORMAL"	,	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "TANGENT"	,	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "BINORMAL",	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT IL_PosTexNorTanBiN::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC IL_Instance::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA	, 0 },
	{ "WORLD",		0, FLOAT4, 1,		0, INSTANCE_DATA, 1 },
	{ "WORLD",		1, FLOAT4, 1, ALIGNED, INSTANCE_DATA, 1 },
	{ "WORLD",		2, FLOAT4, 1, ALIGNED, INSTANCE_DATA, 1 },
	{ "WORLD",		3, FLOAT4, 1, ALIGNED, INSTANCE_DATA, 1 },
};
const UINT IL_Instance::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC IL_Extra_0::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT IL_Extra_0::sElementCount = ARRAYSIZE(sElementDesc);
#pragma endregion

// public
bool InputLayout::CheckInstanced()				{ return mInstanced; }
bool InputLayout::CheckTessellated()			{ return mTessellated; }
ID3D11Buffer* InputLayout::GetVertexBuffer()	{ return mVertexBuffer; }
ID3D11Buffer* InputLayout::GetIndexBuffer()		{ return mIndexBuffer; }
ID3D11Buffer* InputLayout::GetInstanceBuffer()	{ return mInstanceBuffer; }
UINT InputLayout::GetVertexCount()				{ return mVertexCount; }
UINT InputLayout::GetIndexCount()				{ return mIndexCount; }
UINT InputLayout::GetInstanceCount()			{ return mInstanceCount; }
UINT InputLayout::GetDrawCount()				{ return mDrawCount; }
UINT InputLayout::GetInputLayoutSize()			{ return mInputLayoutSize; }
UINT InputLayout::GetInputLayoutInstanceSize()	{ return mInputLayoutInstanceSize; }

void InputLayout::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	D3DXMatrixTranslation(&mWorld, x, y, z);
}

// protected
InputLayout::InputLayout()
{
	D3DXMatrixIdentity(&mWorld);
	mWorldBuffer				= NULL;
	mInstanced					= false;
	mTessellated				= false;
	mVertexBuffer				= NULL;
	mIndexBuffer				= NULL;
	mInstanceBuffer				= NULL;
	mVertexCount				= 0;
	mIndexCount					= 0;
	mInstanceCount				= 0;
	mDrawCount					= 0;
	mInputLayoutSize			= 0;
	mInputLayoutInstanceSize	= 0;

	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_CONSTANT_BUFFER,
		sizeof(WorldBufferData),
		D3D11_USAGE_DYNAMIC,
		NULL,
		&mWorldBuffer
	);
}

InputLayout::~InputLayout()
{
	SafeReleaseCom(mWorldBuffer);
	SafeReleaseCom(mInstanceBuffer);
	SafeReleaseCom(mIndexBuffer);
	SafeReleaseCom(mVertexBuffer);
}

void InputLayout::GetInputLayoutFromFile(wstring meshName)
{
	wstring path = L"Data\\Mesh\\" + meshName + L".txt";

	wifstream file(path.c_str());
	assert(!file.fail());

	WCHAR output;
	file.get(output);
	while (output != '=')
	{
		file.get(output);
	}

	file >> mVertexCount;
	mIndexCount = mVertexCount;
	mTempData.resize(mVertexCount);

	file.get(output);
	for (UINT i = 0; i < mVertexCount; i++)
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 tex;
		D3DXVECTOR3 nor;

		file >> mTempData[i].Pos.x >> mTempData[i].Pos.y >> mTempData[i].Pos.z;
		file >> mTempData[i].Tex.x >> mTempData[i].Tex.y;
		file >> mTempData[i].Nor.x >> mTempData[i].Nor.y >> mTempData[i].Nor.z;
	}

	file.close();
}

void InputLayout::SetWorldBuffer()
{
	D3DXMATRIX world = mWorld;
	D3DXMatrixTranspose(&world, &world);

	WorldBufferData data;
	data.World = world;

	SetConstantBuffer(D3D.GetContext(), mWorldBuffer, &data, sizeof(WorldBufferData), (UINT)RB_World);
}