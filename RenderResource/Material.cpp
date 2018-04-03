#include "Material.h"

// public
Material::Material()
{
	mBuffer			= NULL;
	mAmbient		= D3DXVECTOR4(0.0F, 0.0F, 0.0F, 0.0F);
	mDiffuse		= D3DXVECTOR4(0.0F, 0.0F, 0.0F, 0.0F);
	mSpecular		= D3DXVECTOR4(0.0F, 0.0F, 0.0F, 0.0F);
	mSpecularPower	= 0.0F;
}

Material::~Material()
{
	SafeReleaseCom(mBuffer);
}

void Material::Initialize()
{
	CreateBuffer
	(
		D3D.GetDevice(),
		D3D11_BIND_CONSTANT_BUFFER,
		sizeof(BufferData),
		D3D11_USAGE_DYNAMIC,
		NULL,
		&mBuffer
	);
}

void Material::SetMaterial(D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse, D3DXVECTOR4 specular, FLOAT specularPower)
{
	mAmbient		= ambient;
	mDiffuse		= diffuse;
	mSpecular		= specular;
	mSpecularPower	= specularPower;
}

void Material::SetBuffer(RB_Type materialType)
{
	BufferData data;

	data.Ambient		= mAmbient;
	data.Diffuse		= mDiffuse;
	data.Specular		= mSpecular;
	data.SpecularPower	= mSpecularPower;
	data.Pad			= D3DXVECTOR3(1.0F, 1.0F, 1.0F);

	SetConstantBuffer
	(
		D3D.GetContext(),
		mBuffer,
		&data,
		sizeof(BufferData),
		(UINT)materialType
	);
}