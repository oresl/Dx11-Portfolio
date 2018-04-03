#include "Light.h"

// public
Light::Light()
{
	mBuffer		= NULL;
	mDirection	= D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	mPosition	= D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	mSpeed		= 0.0F;
	mAngle		= 0.0F;
}

Light::~Light()
{
	SafeReleaseCom(mBuffer);
}

void Light::Initialize()
{
	mDirection	= D3DXVECTOR3(0.01F, -1.0F, 0.01F);
	mPosition	= D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	mSpeed		= 10.0F;
	mAngle		= (FLOAT)D3DX_PI / 180.0F * -45.0F;

	// 3D 프로젝션 행렬
	FLOAT fovy = (FLOAT)D3DX_PI / 2.0F;
	FLOAT aspect = 1.0F;

	D3DXMatrixPerspectiveFovLH
	(
		&mProjection,
		fovy,
		aspect,
		gCFEngine.ScreenNear,
		gCFEngine.ScreenDepth
	);

	CreateBuffer(D3D.GetDevice(), D3D11_BIND_CONSTANT_BUFFER, sizeof(BufferData), D3D11_USAGE_DYNAMIC, NULL, &mBuffer);
}

void Light::Update(FLOAT delta)
{
	// 빛의 방향
	mAngle -= (FLOAT)D3DX_PI / 180.0F * delta * mSpeed;

	mDirection.x = cosf(mAngle);
	mDirection.y = sinf(mAngle);
	D3DXVec3Normalize(&mDirection, &mDirection);

	// View행렬
	D3DXVECTOR3 up		= D3DXVECTOR3(0.0F, 1.0F, 0.0F);
	D3DXVECTOR3 lookat	= D3DXVECTOR3(0.0F, 0.0F, 0.0F);	// Terrain의 중심좌표.. 보다는 카메라의 위치좌표가 좋지 않을까..?
	mPosition			= lookat - (mDirection * 300);

	D3DXMatrixLookAtLH(&mView, &mPosition, &lookat, &up);
}

void Light::SetBuffer()
{
	D3DXMATRIX view			= mView;
	D3DXMATRIX projection	= mProjection;

	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	BufferData data;
	data.View		= view;
	data.Projection	= projection;
	data.Direction	= mDirection;
	data.Position	= mPosition;
	data.Pad		= D3DXVECTOR2(1.0F, 1.0F);

	SetConstantBuffer(D3D.GetContext(), mBuffer, &data, sizeof(BufferData), (UINT)RB_Light);
}

D3DXMATRIX Light::GetView()			{ return mView; }
D3DXMATRIX Light::GetProjection()	{ return mProjection; }
D3DXVECTOR3 Light::GetDirection()	{ return mDirection; }
D3DXVECTOR3 Light::GetPosition()	{ return mPosition; }