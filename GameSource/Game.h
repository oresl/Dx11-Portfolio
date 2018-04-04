#pragma once
#include "..\Common\Pch.h"
#include "..\RenderResource\Camera.h"
#include "..\RenderResource\Frustum.h"
#include "..\RenderResource\Light.h"
#include "..\RenderResource\Material.h"
#include "..\RenderResource\RenderManager.h"

#include "Cube.h"
#include "Sphere.h"

#include "SkySphere.h"
#include "SkyPlane.h"
#include "Rain.h"

class Game
{
public:
	Game();
	~Game();
	void			Initialize();
	void			Update(FLOAT delta);
	void			PreRender();
	void			Render();

private:
	bool			mWireFrame;
	
	RenderManager*	mRenderManager;
	Camera*			mCamera;
	Frustum*		mFrustum;
	Light*			mLight;
	Material*		mMaterial0;
	Material*		mMaterial1;

	Cube*			mCube;		// Material0
	Sphere*			mSphere;	// Material1

	SkySphere*		mSkySphere;
	SkyPlane*		mSkyPlane;
	Rain*			mRain;

	void			InitializeRenderResource();
	void			InitializeGameObject();
	void			UpdateRenderResource(FLOAT delta);
	void			UpdateGameObject(FLOAT delta);

};

