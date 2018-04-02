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
	void			UpdateReflection(FLOAT height);

	// Get, Set �Լ�
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

	// ����Ϸ���..
	D3DXMATRIX		GetView();
	D3DXMATRIX		GetProjection();
	D3DXMATRIX		GetOrtho();
	D3DXMATRIX		GetReflection();
	void			SetBuffer();

private:
	struct BufferData
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
		D3DXMATRIX Ortho;
		D3DXMATRIX Reflection;
	};

	D3DXVECTOR3		mPosition;
	FLOAT			mPitch;				// x�� ȸ��
	FLOAT			mYaw;				// y�� ȸ��
	FLOAT			mRoll;				// z�� ȸ��

	FLOAT			mWalkSpeed;
	FLOAT			mStrafeSpeed;
	FLOAT			mJumpSpeed;

	D3DXMATRIX		mView;
	D3DXMATRIX		mProjection;
	D3DXMATRIX		mOrtho;
	D3DXMATRIX		mReflection;
	ID3D11Buffer*	mBuffer;
};