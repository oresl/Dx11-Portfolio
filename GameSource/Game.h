#pragma once
#include "..\Common\Pch.h"
#include "..\RenderResource\Camera.h"
#include "..\RenderResource\Frustum.h"
#include "..\RenderResource\Light.h"
#include "..\RenderResource\Material.h"
#include "..\RenderResource\RenderTexture.h"
#include "..\RenderResource\RenderManager.h"

#include "Cube.h"
#include "Sphere.h"

#include "SkySphere.h"
#include "SkyPlane.h"
#include "Terrain.h"
#include "Rain.h"
#include "Trunk.h"
#include "Leaf.h"

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
	bool			mRained;

	RenderManager*	mRenderManager;
	Camera*			mCamera;
	Frustum*		mFrustum;
	Light*			mLight;
	Material*		mMaterial0;
	Material*		mMaterial1;

	Cube*			mCube;				// Material0
	RenderTexture*	mCubeReflect;

	Sphere*			mSphere;			// Material1
	RenderTexture*	mSphereReflect;

	SkySphere*		mSkySphere;
	RenderTexture*	mSkySphereReflect;

	SkyPlane*		mSkyPlane;
	RenderTexture*	mSkyPlaneReflect;

	Terrain*		mTerrain;
	RenderTexture*	mTrunkShadow;

	Rain*			mRain;

	Trunk*			mTrunk;
	RenderTexture*	mTrunkShadow;
	Leaf*			mLeaf;
	RenderTexture*	mLeafShadow;

	void			InitializeRenderResource();
	void			InitializeGameObject();
	void			UpdateRenderResource(FLOAT delta);
	void			UpdateGameObject(FLOAT delta);

};

