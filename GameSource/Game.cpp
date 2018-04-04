#include "Game.h"

// public
Game::Game()
{
	mWireFrame		= false;
	mRenderManager	= NULL;
	mCamera			= NULL;
	mFrustum		= NULL;
	mLight			= NULL;
	mMaterial0		= NULL;
	mMaterial1		= NULL;
	mCube			= NULL;
	mSphere			= NULL;
	mSkySphere		= NULL;
	mSkyPlane		= NULL;
}

Game::~Game()
{
	SafeDeletePtr(mSkyPlane);
	SafeDeletePtr(mSkySphere);
	SafeDeletePtr(mSphere);
	SafeDeletePtr(mCube);
	SafeDeletePtr(mMaterial1);
	SafeDeletePtr(mMaterial0);
	SafeDeletePtr(mLight);
	SafeDeletePtr(mFrustum);
	SafeDeletePtr(mCamera);
	SafeDeletePtr(mRenderManager);
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
	{
		mRenderManager->SetRasterizerState(RS_CullNone);
		mRenderManager->SetDepthStencilState(DS_DepthDisable);

		mRenderManager->SetShader(L"SkySphere");
		mSkySphere->SetPosition(camPos.x, camPos.y, camPos.z);
		mSkySphere->SetBuffer();
		mRenderManager->SetIAParameterAndDraw(mSkySphere);
	}

	// SkyPlane
	{
		mRenderManager->SetRasterizerState(RS_CullBack);
		mRenderManager->SetBlendState(BS_SkyPlane);

		mRenderManager->SetShader(L"SkyPlane");
		mSkyPlane->SetPosition(camPos.x, camPos.y, camPos.z);
		mSkyPlane->SetBuffer();
		mRenderManager->SetTexture(L"Cloud", 0);
		mRenderManager->SetTexture(L"Noise", 1);
		mRenderManager->SetIAParameterAndDraw(mSkyPlane);
	}
	
	mRenderManager->SetBlendState(BS_Default);
	mRenderManager->SetDepthStencilState(DS_Default);

	// Rain
	{
		mRenderManager->SetRasterizerState(mWireFrame ? RS_WireFrame_CullBack : RS_CullBack);

		mRenderManager->SetShader(L"Rain");
		mRain->SetBuffer();
		mRenderManager->SetIAParameterAndDraw(mRain);
	}

	// Cube (Material0)
	{
		mRenderManager->SetShader(L"Cube");
		mCube->SetBuffer();
		mRenderManager->SetTexture(L"Trapezoid", 0);
		mRenderManager->SetIAParameterAndDraw(mCube);
	}

	// Sphere (Material1)
	{
		mRenderManager->SetShader(L"Sphere");
		mSphere->SetPosition(5.0F, 0.0F, 0.0F);
		mSphere->SetBuffer();
		mRenderManager->SetTexture(L"Earth", 0);
		mRenderManager->SetIAParameterAndDraw(mSphere);
	}
}

// private
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
	mRenderManager = new RenderManager;
	mRenderManager->Initialize();

	mRenderManager->AddShader(L"Cube", IL_PosTexNor::sElementDesc, IL_PosTexNor::sElementCount, false);		// Material 0
	mRenderManager->AddShader(L"Sphere", IL_PosTexNor::sElementDesc, IL_PosTexNor::sElementCount, false);	// Material 1
	mRenderManager->AddShader(L"SkySphere", IL_Pos::sElementDesc, IL_Pos::sElementCount, false);
	mRenderManager->AddShader(L"SkyPlane", IL_PosTex::sElementDesc, IL_PosTex::sElementCount, false);
	mRenderManager->AddShader(L"Rain", IL_Instance::sElementDesc, IL_Instance::sElementCount, false);

	mRenderManager->AddTexture(L"Test", TE_GIF);
	mRenderManager->AddTexture(L"Trapezoid", TE_JPG);
	mRenderManager->AddTexture(L"Earth", TE_PNG);
	mRenderManager->AddTexture(L"Cloud", TE_DDS);
	mRenderManager->AddTexture(L"Noise", TE_DDS);
}

void Game::InitializeGameObject()
{
	mCube = new Cube;
	mCube->Initialize();
	
	mSphere = new Sphere;
	mSphere->Initialize();

	mSkySphere = new SkySphere;
	mSkySphere->Initialize();

	mSkyPlane = new SkyPlane;
	mSkyPlane->Initialize();

	mRain = new Rain;
	mRain->Initialize();
}

void Game::UpdateRenderResource(FLOAT delta)
{
	// Camera, Frustum
	mCamera->Update(delta);
	mCamera->UpdateReflection(0.0F);
	mCamera->SetBuffer();

	D3DXMATRIX view			= mCamera->GetView();
	D3DXMATRIX frustumProj	= mCamera->GetFrustumProj();
	D3DXMATRIX viewProj;
	D3DXMatrixMultiply(&viewProj, &view, &frustumProj);
	mFrustum->SetFrustum(viewProj);

	// Light, Material
	mLight->Update(delta);
	mLight->SetBuffer();

	mMaterial0->SetBuffer(RB_Material0);
	mMaterial1->SetBuffer(RB_Material1);
}

void Game::UpdateGameObject(FLOAT delta)
{
	mSkyPlane->Update(delta);
	mRain->Update(delta, mCamera, mFrustum);
}

