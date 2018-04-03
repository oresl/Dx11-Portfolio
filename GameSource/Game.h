#pragma once
#include "..\Common\Pch.h"
#include "..\RenderResource\Camera.h"
#include "..\RenderResource\Frustum.h"
#include "..\RenderResource\Light.h"
#include "..\RenderResource\Material.h"
#include "..\RenderResource\RenderManager.h"

#include "Cube.h"
#include "SkySphere.h"
#include "SkyPlane.h"

class Game
{
public:
	Game();
	~Game();
	void		Initialize();
	void		Update(FLOAT delta);
	void		PreRender();
	void		Render();

private:
	bool		mWireFrame;
	
	Camera*		mCamera;
	Frustum*	mFrustum;
	Light*		mLight;
	Material*	mMaterial0;
	Material*	mMaterial1;

	Cube*		mCube;
	SkySphere*	mSkySphere;
	SkyPlane*	mSkyPlane;

	void		InitializeRenderResource();
	void		InitializeGameObject();
	void		UpdateRenderResource(FLOAT delta);
	void		UpdateGameObject(FLOAT delta);

};

