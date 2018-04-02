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

void Game::Initialize()
{
	mCamera = new Camera;
	mCamera->Initialize();
	mCamera->SetPosition(0.0F, 0.0F, -10.0F);

	InitializeGameObject();

	RenderMgr.AddShader(L"Cube", IL_PosTexNor::sElementDesc, IL_PosTexNor::sElementCount, false);
}

void Game::Update(FLOAT delta)
{
	mCamera->Update(delta);
	mCamera->UpdateReflection(0.0F);
	mCamera->SetBuffer();
}

void Game::PreRender()
{}

void Game::Render()
{
	RenderMgr.SetShader(L"Cube");
	mCube->SetBuffer();
	RenderMgr.SetIAParameter(mCube);
}

// private
void Game::InitializeGameObject()
{
	mCube = new Cube;
	mCube->Initialize();
}

void Game::InitializeShader()
{

}