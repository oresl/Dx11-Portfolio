#include "Terrain.h"

// public
Terrain::Terrain()
{
	mVertexData.clear();
	mIndexData.clear();
	mBuffer = NULL;
}

Terrain::~Terrain()
{
	SafeReleaseCom(mBuffer);
	mIndexData.clear();
	mVertexData.clear();
}

void Terrain::Initialize()
{
	mInstanced			= false;
	mTessellated		= true;
	mInputLayoutSize	= sizeof(IL_PosTex);

	InitializeData();
	InitializeBuffer();
}

void Terrain::SetBuffer()
{
	SetWorldBuffer();

	BufferData data;
	data.MinCamDist		= gCFTerrain.MinCamDist;
	data.MaxCamDist		= gCFTerrain.MaxCamDist;
	data.MinTessStep	= gCFTerrain.MinTessStep;
	data.MaxTessStep	= gCFTerrain.MaxTessStep;
	data.ClipPlane		= D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0F);
	data.TexelU			= 1.0F / gCFTerrain.Width;
	data.TexelV			= 1.0F / gCFTerrain.Depth;
	data.CellSpacing	= gCFTerrain.CellSpacing;
	data.Pad			= 1.0F;

	SetConstantBuffer(D3D.GetContext(), mBuffer, &data, sizeof(BufferData), (UINT)RB_Custom);
}

ID3D11ShaderResourceView* Terrain::GetHeightmap() { return mHeightmap; }

FLOAT Terrain::GetHeight(FLOAT x, FLOAT z)
{
	// Terrain 범위를 벗어나면 날려버리기!
	if (x < -(GetWidth() * 0.5) || x > GetWidth() * 0.5)
		return 100.0F;
	if (z < -(GetDepth() * 0.5) || z > GetDepth() * 0.5)
		return 100.0F;

	// 셀단위로 변환
	FLOAT c = (x + GetWidth() * 0.5F) / gCFTerrain.CellSpacing;
	FLOAT r = (z - GetDepth() * 0.5F) / -gCFTerrain.CellSpacing;

	// 몇번째 셀인지 계산
	int col = (int)floorf(c);
	int row = (int)floorf(r);

	FLOAT index1 = mHeightmapData[((row + 0) * gCFTerrain.Width) + col];
	FLOAT index2 = mHeightmapData[((row + 0) * gCFTerrain.Width) + col + 1];
	FLOAT index3 = mHeightmapData[((row + 1) * gCFTerrain.Width) + col];
	FLOAT index4 = mHeightmapData[((row + 1) * gCFTerrain.Width) + col + 1];

	FLOAT s = c - (FLOAT)col;
	FLOAT t = r - (FLOAT)row;

	// 도영이꺼보고 확인해봅시다
	if (s + t <= 1.0F)
	{
		FLOAT uy = index2 - index1;
		FLOAT vy = index3 - index1;
		return index1 + s * uy + t * vy;
	}
	else
	{
		FLOAT uy = index3 - index4;
		FLOAT vy = index2 - index4;
		return index4 + (1.0F - s) * uy + (1.0F - t) * vy;
	}
}

// private
void Terrain::InitializeData()
{
	// Heightmap 데이터저장
	{
		wstring path	= L"Data\\Texture\\" + gCFTerrain.Heightmap;
		size_t size		= gCFTerrain.Width * gCFTerrain.Depth;

		vector<BYTE> data;
		GetBYTEFromFile(path, size, &data);

		mHeightmapData.resize(gCFTerrain.Width * gCFTerrain.Depth);

		for (UINT i = 0; i < gCFTerrain.Width * gCFTerrain.Depth; ++i)
		{
			mHeightmapData[i] = (data[i] / 255.0f) * gCFTerrain.Scale;
			mHeightmapData[i] = floorf(mHeightmapData[i]);
		}
	}
	
	// ShaderResourceView생성
	{
		D3D11_TEXTURE2D_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_TEXTURE2D_DESC));	

		srvDesc.Width				= gCFTerrain.Width;
		srvDesc.Height				= gCFTerrain.Depth;
		srvDesc.MipLevels			= 1;
		srvDesc.ArraySize			= 1;
		srvDesc.Format				= DXGI_FORMAT_R32_FLOAT;
		srvDesc.SampleDesc.Count	= 1;
		srvDesc.SampleDesc.Quality	= 0;
		srvDesc.Usage				= D3D11_USAGE_IMMUTABLE;
		srvDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;

		CreateShaderResourceViewFromData(D3D.GetDevice(), srvDesc, &mHeightmapData[0], &mHeightmap);
	}

	// Vertex
	
	// 2^n + 1의 크기의 이미지를 이용해야 quad수가 짝수로 떨어짐
	// tess 1단계 그리드의 Vertex
	// tess step으로 나눈 값에 + 1 = quad의 갯수 (col, row)
	UINT cols = ((gCFTerrain.Width - 1) / gCFTerrain.TessValue) + 1;
	UINT rows = ((gCFTerrain.Depth - 1) / gCFTerrain.TessValue) + 1;

	FLOAT hWidth = GetWidth() * 0.5F;
	FLOAT hDepth = GetDepth() * 0.5F;

	FLOAT pWidth = GetWidth() / (cols - 1);
	FLOAT pDepth = GetDepth() / (rows - 1);

	// fx에서 SampleLevel을 통해 y값 추출을 하기위해 quad크기에 맞는 좌표를 넘겨줘야함
	FLOAT u = 1.0F / (cols - 1);
	FLOAT v = 1.0F / (rows - 1);

	mVertexCount = cols * rows;
	mVertexData.resize(mVertexCount);

	for (UINT z = 0; z < rows; z++)
	{
		FLOAT posZ = hDepth - (z * pDepth);

		for (UINT x = 0; x < cols; x++)
		{
			FLOAT posX = -hWidth + (x * pWidth);
			int index = (z * cols) + x;

			mVertexData[index].Pos.x = posX;
			mVertexData[index].Pos.y = 0.0F;
			mVertexData[index].Pos.z = posZ;

			mVertexData[index].Tex.x = (FLOAT)x * u;
			mVertexData[index].Tex.y = (FLOAT)z * v;
		}
	}

	// TODO:: Bounds값 계산해서 넘겨줘야함 (Frustum관련)

	// Index
	mIndexCount = (cols - 1) * (rows - 1) * 4;
	mIndexData.resize(mIndexCount);

	int k = 0;
	for (UINT z = 0; z < rows - 1; z++)
	{
		for (UINT x = 0; x < cols - 1; x++)
		{
			mIndexData[k + 0] = ((z + 0) * cols) + x;
			mIndexData[k + 1] = ((z + 0) * cols) + x + 1;
			mIndexData[k + 2] = ((z + 1) * cols) + x;
			mIndexData[k + 3] = ((z + 1) * cols) + x + 1;

			k += 4;
		}
	}
}

void Terrain::InitializeBuffer()
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
		D3D11_USAGE_DYNAMIC,
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

// Cell의 크기에 따른 전체크기 조절
FLOAT Terrain::GetWidth() { return (gCFTerrain.Width - 1) * gCFTerrain.CellSpacing; }
FLOAT Terrain::GetDepth() { return (gCFTerrain.Depth - 1) * gCFTerrain.CellSpacing; }