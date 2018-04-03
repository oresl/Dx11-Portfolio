#include "Game.h"

// public
Game::Game()
{
	mWireFrame	= false;

	mCamera		= NULL;
	mCube		= NULL;
}

Game::~Game()
{
	SafeDeletePtr(mCamera);
}

void Game::Initialize()
{
	InitializeRenderResource();
	InitializeGameObject();
}

void Game::Update(FLOAT delta)
{
	UpdateRenderResource(delta);
	UpdateGameObject(delta);

	if (Input.KeyUp(DIK_1))
		mWireFrame = !mWireFrame;

	if (Input.KeyUp(DIK_2))
		mWireFrame = !mWireFrame;

	if (Input.KeyUp(DIK_ESCAPE))
		PostQuitMessage(0);
}

void Game::PreRender()
{}

void Game::Render()
{
	D3DXVECTOR3 camPos = mCamera->GetPosition();

	// SkySphere
	RenderMgr.SetRasterizerState(RS_CullNone);
	RenderMgr.SetDepthStencilState(DS_DepthDisable);
	RenderMgr.SetShader(L"SkySphere");
	mSkySphere->SetPosition(camPos.x, camPos.y, camPos.z);
	mSkySphere->SetBuffer();
	RenderMgr.SetIAParameter(mSkySphere);

	// SkyPlane
	RenderMgr.SetRasterizerState(RS_CullBack);
	RenderMgr.SetBlendState(BS_SkyPlane);
	RenderMgr.SetShader(L"SkyPlane");
	mSkyPlane->SetPosition(camPos.x, camPos.y, camPos.z);
	mSkyPlane->SetBuffer();
	RenderMgr.SetTexture(L"Cloud", 0);
	RenderMgr.SetTexture(L"Noise", 1);
	RenderMgr.SetIAParameter(mSkyPlane);
	RenderMgr.SetBlendState(BS_Default);
	RenderMgr.SetDepthStencilState(DS_Default);

	// Cube0
	RenderMgr.SetShader(L"Cube");
	RenderMgr.SetRasterizerState(mWireFrame ? RS_WireFrame_CullBack : RS_CullBack);
	mCube->SetBuffer();
	RenderMgr.SetTexture(L"Trapezoid", 0);
	RenderMgr.SetIAParameter(mCube);
}

// private
void Game::InitializeGameObject()
{
	mCube = new Cube;
	mCube->Initialize();
	
	mSkySphere = new SkySphere;
	mSkySphere->Initialize();

	mSkyPlane = new SkyPlane;
	mSkyPlane->Initialize();
}

void Game::InitializeRenderResource()
{
	// Camera, Frustum
	mCamera = new Camera;
	mCamera->Initialize();
	mCamera->SetPosition(0.0F, 0.0F, -10.0F);

	mFrustum = new Frustum;

	// Light, Material
	mLight = new Light;
	mLight->Initialize();

	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	FLOAT		specularPower;

	// Specular x
	ambient			= D3DXVECTOR4(0.15F, 0.15F, 0.15F, 1.0F);
	diffuse			= D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0F);
	specular		= D3DXVECTOR4(0.0F, 0.0F, 0.0F, 1.0F);
	specularPower	= 0.0F;

	mMaterial0 = new Material;
	mMaterial0->Initialize();
	mMaterial0->SetMaterial(ambient, diffuse, specular, specularPower);

	// Specular o
	ambient			= D3DXVECTOR4(0.15F, 0.15F, 0.15F, 1.0F);
	diffuse			= D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0F);
	specular		= D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0F);
	specularPower	= 32.0F;

	mMaterial1 = new Material;
	mMaterial1->Initialize();
	mMaterial1->SetMaterial(ambient, diffuse, specular, specularPower);

	// Shader, Texture
	RenderMgr.Initialize();

	RenderMgr.AddShader(L"Cube", IL_PosTexNor::sElementDesc, IL_PosTexNor::sElementCount, false);
	RenderMgr.AddShader(L"SkySphere", IL_Pos::sElementDesc, IL_Pos::sElementCount, false);
	RenderMgr.AddShader(L"SkyPlane", IL_PosTex::sElementDesc, IL_PosTex::sElementCount, false);

	RenderMgr.AddTexture(L"Test", TE_GIF);
	RenderMgr.AddTexture(L"Trapezoid", TE_JPG);
	RenderMgr.AddTexture(L"Cloud", TE_DDS);
	RenderMgr.AddTexture(L"Noise", TE_DDS);
}

void Game::UpdateGameObject(FLOAT delta)
{
	mSkyPlane->Update(delta);
}

void Game::UpdateRenderResource(FLOAT delta)
{
	// Camera, Frustum
	mCamera->Update(delta);
	mCamera->UpdateReflection(0.0F);
	mCamera->SetBuffer();

	D3DXMATRIX view			= mCamera->GetView();
	D3DXMATRIX projection	= mCamera->GetProjection();
	D3DXMATRIX viewProj;
	D3DXMatrixMultiply(&viewProj, &view, &projection);
	mFrustum->SetFrustum(viewProj);

	// Light, Material
	mLight->Update(delta);
	mLight->SetBuffer();

	mMaterial0->SetBuffer(RB_Material0);
	mMaterial1->SetBuffer(RB_Material1);
}
