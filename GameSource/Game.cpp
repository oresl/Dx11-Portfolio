#include "Game.h"

// public
Game::Game()
{
	mWireFrame		= false;
	mRained			= false;
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
		mRained = !mRained;

	if (Input.KeyUp(DIK_ESCAPE))
		PostQuitMessage(0);
}

void Game::PreRender()
{
}

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
		mRenderManager->SetTexture(L"Cloud", 0);
		mRenderManager->SetTexture(L"Noise", 1);
		mSkyPlane->SetPosition(camPos.x, camPos.y, camPos.z);
		mSkyPlane->SetBuffer();
		mRenderManager->SetIAParameterAndDraw(mSkyPlane);
	}
	
	mRenderManager->SetBlendState(BS_Default);
	mRenderManager->SetDepthStencilState(DS_Default);

	// Terrain
	{
		mRenderManager->SetRasterizerState(mWireFrame ? RS_WireFrame_CullBack : RS_CullBack);

		mRenderManager->SetShader(L"Terrain");
		mRenderManager->SetTexture(mTerrain->GetHeightmap(), 0);
		mRenderManager->SetTexture(L"Blendmap", 1);
		mRenderManager->SetTextureArray(L"LayermapArray", 2);
		mTerrain->SetBuffer();
		mRenderManager->SetIAParameterAndDraw(mTerrain);
	}

	//mRenderManager->SetRasterizerState(RS_Default);

	// TODO:: Normal값 적용할 것
	// Tree
	{
		// Trunk
		vector<wstring> imgTrunk;
		vector<wstring> imgLeaf;
		imgTrunk.push_back(L"Trunk1");	imgTrunk.push_back(L"Trunk2");	imgTrunk.push_back(L"Trunk3");
		imgLeaf.push_back(L"Leaf1");	imgLeaf.push_back(L"Leaf2");	imgLeaf.push_back(L"Leaf3");

		for (int i = 0; i < gCFTree.Sort; i++)
		{
			mRenderManager->SetShader(L"Trunk");
			mRenderManager->SetTexture(imgTrunk[i].c_str(), 0);
			mTrunk[i].SetBuffer();
			mRenderManager->SetIAParameterAndDraw(&mTrunk[i]);
		}
	
		// Leaf
		mRenderManager->SetBlendState(BS_Alpha);

		for (int i = 0; i < gCFTree.Sort; i++)
		{
			mRenderManager->SetShader(L"Leaf");
			mRenderManager->SetTexture(imgLeaf[i].c_str(), 0);
			mLeaf[i].SetBuffer();
			mRenderManager->SetIAParameterAndDraw(&mLeaf[i]);
		}
	}

	mRenderManager->SetBlendState(BS_Default);

	// Rain
	if (mRained)
	{
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
	mRenderManager->AddShader(L"Terrain", IL_PosTex::sElementDesc, IL_PosTex::sElementCount, true);
	mRenderManager->AddShader(L"Rain", IL_PosInst::sElementDesc, IL_PosInst::sElementCount, false);
	mRenderManager->AddShader(L"Trunk", IL_PosTexInst::sElementDesc, IL_PosTexInst::sElementCount, false);
	mRenderManager->AddShader(L"Leaf", IL_PosTexInst::sElementDesc, IL_PosTexInst::sElementCount, false);

	mRenderManager->AddTexture(L"Test", TE_GIF);
	mRenderManager->AddTexture(L"Trapezoid", TE_JPG);
	mRenderManager->AddTexture(L"Earth", TE_PNG);
	mRenderManager->AddTexture(L"Cloud", TE_DDS);
	mRenderManager->AddTexture(L"Noise", TE_DDS);

	mRenderManager->AddTexture(L"Blendmap", TE_DDS);
	vector<wstring> layermap;
	wstring path = L"Data\\Texture\\";
	layermap.push_back(path + gCFTerrain.Layermap0);
	layermap.push_back(path + gCFTerrain.Layermap1);
	layermap.push_back(path + gCFTerrain.Layermap2);
	layermap.push_back(path + gCFTerrain.Layermap3);
	mRenderManager->AddTextureArray(L"LayermapArray", layermap, TE_DDS);

	mRenderManager->AddTexture(L"Trunk1", TE_DDS);
	mRenderManager->AddTexture(L"Trunk2", TE_DDS);
	mRenderManager->AddTexture(L"Trunk3", TE_DDS);
	mRenderManager->AddTexture(L"Leaf1", TE_DDS);
	mRenderManager->AddTexture(L"Leaf2", TE_DDS);
	mRenderManager->AddTexture(L"Leaf3", TE_DDS);
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

	mTerrain = new Terrain;
	mTerrain->Initialize();

	mRain = new Rain;
	mRain->Initialize();

	mTrunk	= new Trunk[gCFTree.Sort];
	mLeaf	= new Leaf[gCFTree.Sort];
	
	FLOAT scale = 0.2F;
	for (int i = 0; i < gCFTree.Sort; i++)
	{
		mTrunk[i].Initialize(mTerrain, scale);
		mLeaf[i].Initialize(&mTrunk[i], scale);
		scale += 0.1F;
	}
}

void Game::UpdateRenderResource(FLOAT delta)
{
	// Camera, Frustum
	mCamera->Update(delta);
	mCamera->UpdateReflection(0.0F);
	mCamera->SetBuffer();

	// TODO:: Frustum이 이상합니다.. 내가 이상한 것 같지만 일단 이상합니다
	D3DXMATRIX view = mCamera->GetView();
	D3DXMATRIX proj = mCamera->GetProjection();
	D3DXMATRIX viewProj;
	D3DXMatrixMultiply(&viewProj, &view, &proj);
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
	
	if (mRained)
		mRain->Update(delta, mCamera, mFrustum);

	for (int i = 0; i < gCFTree.Sort; i++)
	{
		mTrunk[i].Update(mFrustum);
		mLeaf[i].Update(mFrustum);
	}
}

