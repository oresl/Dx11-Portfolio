#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Camera.h"
#include "..\RenderResource\RenderManager.h"
#include "Cube.h"

class Game
{
public:
	Game();
	~Game();
	void Initialize();
	void Update(FLOAT delta);
	void PreRender();
	void Render();

private:
	bool mWireFrame;

	Camera* mCamera;
	Cube* mCube;

	void InitializeRenderManager();
	void InitializeGameObject();
};

