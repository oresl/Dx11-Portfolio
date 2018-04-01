#include "VertexLayout.h"
#define FLOAT1			DXGI_FORMAT_R32_FLOAT
#define FLOAT2			DXGI_FORMAT_R32G32_FLOAT
#define FLOAT3			DXGI_FORMAT_R32G32B32_FLOAT
#define FLOAT4			DXGI_FORMAT_R32G32B32A32_FLOAT
#define VERTEX_DATA		D3D11_INPUT_PER_VERTEX_DATA
#define INSTANCE_DATA	D3D11_INPUT_PER_INSTANCE_DATA
#define ALIGNED			D3D11_APPEND_ALIGNED_ELEMENT

const D3D11_INPUT_ELEMENT_DESC VL_Pos::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
};
const UINT VL_Pos::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC VL_PosTex::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT2, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT VL_PosTex::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC VL_PosTexNor::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT2, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "NORMAL"	,	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT VL_PosTexNor::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC VL_PosTexNorTanBiN::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT2, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "NORMAL"	,	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "TANGENT"	,	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
	{ "BINORMAL",	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT VL_PosTexNorTanBiN::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC VL_Instance::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA	, 0 },
	{ "WORLD",		0, FLOAT4, 1,		0, INSTANCE_DATA, 1 },
	{ "WORLD",		1, FLOAT4, 1, ALIGNED, INSTANCE_DATA, 1 },
	{ "WORLD",		2, FLOAT4, 1, ALIGNED, INSTANCE_DATA, 1 },
	{ "WORLD",		3, FLOAT4, 1, ALIGNED, INSTANCE_DATA, 1 },
};
const UINT VL_Instance::sElementCount = ARRAYSIZE(sElementDesc);

const D3D11_INPUT_ELEMENT_DESC VL_Extra_0::sElementDesc[] =
{
	{ "POSITION",	0, FLOAT3, 0,		0, VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, FLOAT3, 0, ALIGNED, VERTEX_DATA, 0 },
};
const UINT VL_Extra_0::sElementCount = ARRAYSIZE(sElementDesc);

// protected
VertexLayout::VertexLayout()
{
	mTempData.clear();
}

VertexLayout::~VertexLayout()
{
	mTempData.clear();
}

void VertexLayout::CreateMeshDataFromFile(wstring meshName)
{
	mTempData.clear();

	wstring path = L"Data\\Mesh\\" + meshName + L".txt";

	wifstream file(path.c_str());
	assert(!file.fail());

	WCHAR output;
	file.get(output);
	while (output != '=')
	{
		file.get(output);
	}
	UINT vertexCount;
	file >> vertexCount;

	mTempData.resize(vertexCount);

	file.get(output);
	for (UINT i = 0; i < vertexCount; i++)
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 tex;
		D3DXVECTOR3 nor;

		file >> mTempData[i].Pos.x >> mTempData[i].Pos.y >> mTempData[i].Pos.z;
		file >> mTempData[i].UV.x  >> mTempData[i].UV.y;
		file >> mTempData[i].Nor.x >> mTempData[i].Nor.y >> mTempData[i].Nor.z;
	}

	file.close();
}

void VertexLayout::CreateBuffer(D3D11_BIND_FLAG bindFlag, UINT size, D3D11_USAGE usage, void* data, ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = bindFlag;
	vbDesc.ByteWidth = size;
	vbDesc.Usage = usage;

	if (usage == D3D11_CPU_ACCESS_WRITE)
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		vbDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = data;

	HRESULT hr = D3D.GetDevice()->CreateBuffer(&vbDesc, &vbData, buffer);
	assert(SUCCEEDED(hr));
}