#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"

class Material
{
public:
	Material();
	~Material();
	void			Initialize();
	void			SetMaterial(D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse, D3DXVECTOR4 specular, FLOAT specularPower);
	void			SetBuffer(RB_Type materialType);

private:
	struct BufferData
	{
		D3DXVECTOR4		Ambient;
		D3DXVECTOR4		Diffuse;
		D3DXVECTOR4		Specular;
		FLOAT			SpecularPower;
		D3DXVECTOR3		Pad;
	};

	ID3D11Buffer*	mBuffer;
	D3DXVECTOR4		mAmbient;
	D3DXVECTOR4		mDiffuse;
	D3DXVECTOR4		mSpecular;
	FLOAT			mSpecularPower;
};