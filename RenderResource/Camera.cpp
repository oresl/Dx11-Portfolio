#include "Camera.h"

// public
Camera::Camera()
{
	mBuffer			= NULL;
	mPosition		= D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	mPitch			= 0.0F;
	mYaw			= 0.0F;
	mRoll			= 0.0F;
	mWalkSpeed		= 0.0F;
	mStrafeSpeed	= 0.0F;
	mJumpSpeed		= 0.0F;
}

Camera::~Camera()
{
	SafeReleaseCom(mBuffer);
}

void Camera::Initialize()
{
	// 3D 프로젝션 행렬
	FLOAT fovy		= (FLOAT)D3DX_PI / 4.0F;
	FLOAT aspect	= (FLOAT)gCFEngine.ScreenX / (FLOAT)gCFEngine.ScreenY;

	D3DXMatrixPerspectiveFovLH
	(
		&mProjection,
		fovy,
		aspect,
		gCFEngine.ScreenNear,
		gCFEngine.ScreenDepth
	);

	// 2D 프로젝션 행렬
	D3DXMatrixOrthoLH
	(
		&mOrtho,
		(FLOAT)gCFEngine.ScreenX,
		(FLOAT)gCFEngine.ScreenY,
		gCFEngine.ScreenNear,
		gCFEngine.ScreenDepth
	);

	// 버퍼 생성
	CreateBuffer(D3D.GetDevice(), D3D11_BIND_CONSTANT_BUFFER, sizeof(BufferData), D3D11_USAGE_DYNAMIC, NULL, &mBuffer);
}

void Camera::Update(FLOAT delta)
{
#pragma region 카메라 움직임 업데이트
	FLOAT speed = gCFEngine.CameraSpeed * delta;

	if (Input.KeyDown(DIK_W))
		mWalkSpeed += speed;
	if (Input.KeyDown(DIK_S))
		mWalkSpeed -= speed;

	if (Input.KeyDown(DIK_A))
		mStrafeSpeed -= speed;
	if (Input.KeyDown(DIK_D))
		mStrafeSpeed += speed;

	if (Input.KeyDown(DIK_SPACE))
		mJumpSpeed += speed;
	if (Input.KeyDown(DIK_LCONTROL))
		mJumpSpeed -= speed;

	// 아크볼 회전
	if (Input.ButtonDown(DIM_LBUTTON) && (Input.GetMouseStatelX() || Input.GetMouseStatelY()))
	{
		// 2018.02.27 회전움직임에 delta로 계산하니까 끊김현상이 발생
		mPitch	+= Input.GetCurrentMouselY()  * 0.001F;
		mYaw	+= Input.GetPreviousMouselX() * 0.001F;
	}
#pragma endregion

#pragma region 행렬 업데이트
	D3DXVECTOR3 right	= D3DXVECTOR3(1.0F, 0.0F, 0.0F);
	D3DXVECTOR3 up		= D3DXVECTOR3(0.0F, 1.0F, 0.0F);
	D3DXVECTOR3 lookat	= D3DXVECTOR3(0.0F, 0.0F, 1.0F);

	D3DXMATRIX rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, mYaw, mPitch, mRoll);

	D3DXVec3TransformCoord(&right, &right, &rotation);
	D3DXVec3TransformCoord(&lookat, &lookat, &rotation);
	D3DXVec3Cross(&up, &lookat, &right);

	mPosition		+= mWalkSpeed	* lookat;
	mPosition		+= mStrafeSpeed * right;
	mPosition		+= mJumpSpeed	* up;

	mWalkSpeed		= 0.0F;
	mStrafeSpeed	= 0.0F;
	mJumpSpeed		= 0.0F;

	lookat			+= mPosition;

	D3DXMatrixLookAtLH(&mView, &mPosition, &lookat, &up);
#pragma endregion
}

void Camera::UpdateReflection(FLOAT height)
{
	//D3DXVECTOR3 right	= D3DXVECTOR3(1.0F, 0.0F, 0.0F);
	//D3DXVECTOR3 up		= D3DXVECTOR3(0.0F, 1.0F, 0.0F);
	//D3DXVECTOR3 lookat	= D3DXVECTOR3(0.0F, 0.0F, 1.0F);

	//D3DXMATRIX rotation;
	//D3DXMatrixRotationYawPitchRoll(&rotation, mYaw, -mPitch, mRoll);

	//D3DXVec3TransformCoord(&right, &right, &rotation);
	//D3DXVec3TransformCoord(&lookat, &lookat, &rotation);
	//D3DXVec3Cross(&up, &lookat, &right);

	//D3DXVECTOR3 position;
	//position.x =  mPosition.x;
	//position.y = -mPosition.y + height;
	//position.z =  mPosition.z;

	//D3DXMatrixLookAtLH(&mReflection, &position, &lookat, &up);
}

void Camera::SetBuffer()
{
	D3DXMATRIX view			= mView;
	D3DXMATRIX projection	= mProjection;
	D3DXMATRIX ortho		= mOrtho;
	D3DXMATRIX reflection	= mReflection;

	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);
	D3DXMatrixTranspose(&ortho, &ortho);
	D3DXMatrixTranspose(&reflection, &reflection);

	BufferData data;
	
	data.View				= view;
	data.Projection			= projection;
	data.Ortho				= ortho;
	data.Reflection			= reflection;

	SetConstantBuffer(D3D.GetContext(), mBuffer, &data, sizeof(BufferData), (UINT)RB_Camera);
}

D3DXVECTOR3 Camera::GetPosition() 					{ return mPosition; }
FLOAT Camera::GetPitch()							{ return mPitch; }
FLOAT Camera::GetYaw()								{ return mYaw; }
FLOAT Camera::GetRoll()								{ return mRoll; }
void Camera::SetPosition(FLOAT x, FLOAT y, FLOAT z) { mPosition = D3DXVECTOR3(x, y, z); }
void Camera::SetPitch(FLOAT angle)					{ mPitch = angle; }
void Camera::SetYaw(FLOAT angle)					{ mYaw = angle; }
void Camera::SetRoll(FLOAT angle)					{ mRoll = angle; }

FLOAT Camera::GetWalkSpeed()						{ return mWalkSpeed; }
FLOAT Camera::GetStrafeSpeed()						{ return mStrafeSpeed; }
FLOAT Camera::GetJumpSpeed()						{ return mJumpSpeed; }
void Camera::SetWalkSpeed(FLOAT speed)				{ mWalkSpeed = speed; }
void Camera::SetStrafeSpeed(FLOAT speed)			{ mStrafeSpeed = speed; }
void Camera::SetJumpSpeed(FLOAT speed)				{ mJumpSpeed = speed; }

D3DXMATRIX Camera::GetView()						{ return mView; }
D3DXMATRIX Camera::GetProjection()					{ return mProjection; }
D3DXMATRIX Camera::GetOrtho()						{ return mOrtho; }
D3DXMATRIX Camera::GetReflection()					{ return mReflection; }