#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\InputLayout.h"

class Sphere : public InputLayout
{
public:
	Sphere();
	~Sphere();
	void					Initialize();
	void					SetBuffer();

private:
	vector<IL_PosTexNor>	mVertexData;
	vector<UINT>			mIndexData;

	virtual void			InitializeData();
	virtual void			InitializeBuffer();
};