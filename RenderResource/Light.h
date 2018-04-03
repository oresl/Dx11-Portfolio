#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"

class Light
{
public:
	Light();
	~Light();
	void			Initialize();
	void			Update(FLOAT delta);
	void			SetBuffer();

	// Get
	D3DXMATRIX		GetView();
	D3DXMATRIX		GetProjection();
	D3DXVECTOR3		GetDirection();
	D3DXVECTOR3		GetPosition();

private:
	struct BufferData
	{
		D3DXMATRIX		View;
		D3DXMATRIX		Projection;
		D3DXVECTOR3		Direction;
		D3DXVECTOR3		Position;
		D3DXVECTOR2		Pad;
	};

	D3DXMATRIX		mView;
	D3DXMATRIX		mProjection;
	D3DXVECTOR3		mDirection;
	D3DXVECTOR3		mPosition;
	FLOAT			mSpeed;
	FLOAT			mAngle;
	ID3D11Buffer*	mBuffer;
};