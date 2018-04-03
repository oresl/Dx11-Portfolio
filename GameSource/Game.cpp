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
	mCamera = new Camera;
	mCamera->Initialize();
	mCamera->SetPosition(0.0F, 0.0F, -10.0F);

	InitializeRenderManager();
	InitializeGameObject();
}

void Game::Update(FLOAT delta)
{
	mCamera->Update(delta);
	mCamera->UpdateReflection(0.0F);
	mCamera->SetBuffer();

	if (Input.KeyUp(DIK_1))
		mWireFrame = !mWireFrame;

	if (Input.KeyUp(DIK_ESCAPE))
		PostQuitMessage(0);
}

void Game::PreRender()
{}

void Game::Render()
{
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
}

void Game::InitializeRenderManager()
{
	RenderMgr.Initialize();

	RenderMgr.AddShader(L"Cube", IL_PosTexNor::sElementDesc, IL_PosTexNor::sElementCount, false);

	RenderMgr.AddTexture(L"Test", TE_GIF);
	RenderMgr.AddTexture(L"Trapezoid", TE_JPG);
}