#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::SetFrustum(D3DXMATRIX viewproj)
{
	D3DXVECTOR3 vertex[8];

	//vertex[0] = D3DXVECTOR3(-1.0F, -1.0F, 0.0F);
	//vertex[1] = D3DXVECTOR3( 1.0F, -1.0F, 0.0F);
	//vertex[2] = D3DXVECTOR3( 1.0F, -1.0F, 1.0F);
	//vertex[3] = D3DXVECTOR3(-1.0F, -1.0F, 1.0F);
	//vertex[4] = D3DXVECTOR3(-1.0F,  1.0F, 0.0F);
	//vertex[5] = D3DXVECTOR3( 1.0F,  1.0F, 0.0F);
	//vertex[6] = D3DXVECTOR3( 1.0F,  1.0F, 1.0F);
	//vertex[7] = D3DXVECTOR3(-1.0F,  1.0F, 1.0F);

	vertex[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[1] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[2] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[3] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertex[5] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertex[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertex[7] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);

	D3DXMATRIXA16 matInv;
	D3DXMatrixInverse(&matInv, NULL, &viewproj);

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&vertex[i], &vertex[i], &matInv);
	}

	D3DXPlaneFromPoints(&mPlane[0], &vertex[5], &vertex[6], &vertex[7]);		// 원 평면(far)
	D3DXPlaneFromPoints(&mPlane[1], &vertex[0], &vertex[4], &vertex[7]);		// 좌 평면(left)
	D3DXPlaneFromPoints(&mPlane[2], &vertex[1], &vertex[2], &vertex[6]);		// 우 평면(right)

	//D3DXPlaneFromPoints(&mPlane[0], &vertex[2], &vertex[6], &vertex[7]); // 원
	//D3DXPlaneFromPoints(&mPlane[1], &vertex[0], &vertex[3], &vertex[7]); // 좌
	//D3DXPlaneFromPoints(&mPlane[2], &vertex[1], &vertex[5], &vertex[6]); // 우
	//D3DXPlaneFromPoints(&mPlane[3], &vertex[4], &vertex[7], &vertex[6]); // 상
	//D3DXPlaneFromPoints(&mPlane[4], &vertex[0], &vertex[1], &vertex[2]); // 하
	//D3DXPlaneFromPoints(&mPlane[5], &vertex[0], &vertex[4], &vertex[5]); // 근
}

bool Frustum::CheckPoint(D3DXVECTOR3 position)
{
	FLOAT dist = 0.0F;

	for (int i = 0; i < 3; i++)
	{
		dist = D3DXPlaneDotCoord(&mPlane[i], &position);
		if (dist > PLANE_ELIPSON)
			return false;
	}

	return true;
}

bool Frustum::CheckShpere(D3DXVECTOR3 position, FLOAT radius)
{
	FLOAT dist = 0.0F;

	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
	for (int i = 0; i < 3; i++)
	{
		dist = D3DXPlaneDotCoord(&mPlane[i], &position);
		if (dist > radius + PLANE_ELIPSON)
			return false;
	}

	return true;
}