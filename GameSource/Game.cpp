#include "Game.h"

// public
Game::Game()
{
	mCamera = NULL;
}

Game::~Game()
{
	SafeDeletePtr(mCamera);
}


ID3D11VertexShader* shaderVS = NULL;
ID3D11PixelShader*  shaderPS = NULL;
ID3D11InputLayout*  inputlayout = NULL;
ID3D11Buffer* cameraBuff = NULL;
struct BufferData
{
	D3DXMATRIX	World;
	D3DXMATRIX	View;
	D3DXMATRIX	Proj;
};

D3DXMATRIX tWorld;
D3DXMATRIX tView;
D3DXMATRIX tProj;

void Game::Initialize()
{
	mCamera = new Camera;
	mCamera->Initialize();
	mCamera->SetPosition(0.0F, 0.0F, -10.0F);

	InitializeGameObject();

	// юс╫ц
	ID3D10Blob* vsBlob = NULL;
	ID3D10Blob* psBlob = NULL;
	ID3D10Blob* error = NULL;
	HRESULT hr = D3DX11CompileFromFile
	(
		L"GameSource\\Cube.fx",
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&vsBlob,
		&error,
		NULL
	);
	assert(SUCCEEDED(hr));
	hr = D3D.GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &shaderVS);
	assert(SUCCEEDED(hr));

	hr = D3DX11CompileFromFile
	(
		L"GameSource\\Cube.fx",
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&psBlob,
		NULL,
		NULL
	);
	assert(SUCCEEDED(hr));
	hr = D3D.GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &shaderPS);
	assert(SUCCEEDED(hr));

	D3D11_INPUT_ELEMENT_DESC elementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT elementCount = ARRAYSIZE(elementDesc);
	D3D.GetDevice()->CreateInputLayout
	(
		elementDesc,
		elementCount,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputlayout
	);
	assert(SUCCEEDED(hr));

	D3DXMatrixIdentity(&tWorld);
	D3DXMatrixIdentity(&tView);
	D3DXMatrixIdentity(&tProj);

	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));

	buffDesc.Usage					= D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth				= sizeof(BufferData);
	buffDesc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags				= 0;
	buffDesc.StructureByteStride	= 0;

	hr = D3D.GetDevice()->CreateBuffer(&buffDesc, NULL, &cameraBuff);
	assert(SUCCEEDED(hr));
}

void Game::Update(FLOAT delta)
{
	mCamera->Update(delta);
}

void Game::PreRender()
{}

void Game::Render()
{
	D3DXMatrixIdentity(&tWorld);
	tView = mCamera->GetView();
	tProj = mCamera->GetProjection();

	// Set Shader
	D3D.GetContext()->IASetInputLayout(inputlayout);
	D3D.GetContext()->VSSetShader(shaderVS, NULL, 0);
	D3D.GetContext()->PSSetShader(shaderPS, NULL, 0);
	
	// Update Buffer
	D3DXMatrixTranspose(&tWorld, &tWorld);
	D3DXMatrixTranspose(&tView, &tView);
	D3DXMatrixTranspose(&tProj, &tProj);

	D3D11_MAPPED_SUBRESOURCE mapped;
	D3D.GetContext()->Map(cameraBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	BufferData* data = (BufferData*)mapped.pData;
	data->World = tWorld;
	data->View	= tView;
	data->Proj	= tProj;

	D3D.GetContext()->Unmap(cameraBuff, 0);
	D3D.GetContext()->VSSetConstantBuffers(0, 1, &cameraBuff);

	// Update IA Buffer
	UINT strides = sizeof(VL_PosTexNor);
	UINT offsets = 0;
	
	ID3D11Buffer* buffV = mCube->GetVertexBuffer();
	ID3D11Buffer* buffI = mCube->GetIndexBuffer();

	D3D.GetContext()->IASetVertexBuffers(0, 1, &buffV, &strides, &offsets);
	D3D.GetContext()->IASetIndexBuffer(buffI, DXGI_FORMAT_R32_UINT, 0);
	
	// Set Topology and Draw
	D3D.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D.GetContext()->DrawIndexed(36, 0, 0);
}

// private
void Game::InitializeGameObject()
{
	mCube = new Cube;
	mCube->Initialize();
}