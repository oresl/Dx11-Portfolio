#pragma once
#include "..\Common\Pch.h"
#define PLANE_ELIPSON 5.0F

class Frustum
{
public:
	Frustum();
	~Frustum();
	void SetFrustum(D3DXMATRIX viewproj);
	bool CheckPoint(D3DXVECTOR3 position);
	bool CheckShpere(D3DXVECTOR3 position, FLOAT radius);

private:
	D3DXPLANE mPlane[6];

};