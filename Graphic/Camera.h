#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\System\DirectInput.h"

class Camera
{
public:
	Camera();
	~Camera();
	void			Initialize();
	void			Update(FLOAT delta);
	void			UpdateReflection();

	// Get, Set 함수
	D3DXVECTOR3		GetPosition();
	FLOAT			GetPitch();
	FLOAT			GetYaw();
	FLOAT			GetRoll();
	void			SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void			SetPitch(FLOAT angle);
	void			SetYaw(FLOAT angle);
	void			SetRoll(FLOAT angle);

	FLOAT			GetWalkSpeed();
	FLOAT			GetStrafeSpeed();
	FLOAT			GetJumpSpeed();
	void			SetWalkSpeed(FLOAT speed);
	void			SetStrafeSpeed(FLOAT speed);
	void			SetJumpSpeed(FLOAT speed);

	D3DXMATRIX		GetView();
	D3DXMATRIX		GetProjection();
	D3DXMATRIX		GetOrthoProjection();
	D3DXMATRIX		GetReflection();

private:
	D3DXVECTOR3		mPosition;
	FLOAT			mPitch;				// x축 회전
	FLOAT			mYaw;				// y축 회전
	FLOAT			mRoll;				// z축 회전

	FLOAT			mWalkSpeed;
	FLOAT			mStrafeSpeed;
	FLOAT			mJumpSpeed;

	D3DXMATRIX		mView;
	D3DXMATRIX		mProjection;
	D3DXMATRIX		mOrthoProjection;
	D3DXMATRIX		mReflection;

};