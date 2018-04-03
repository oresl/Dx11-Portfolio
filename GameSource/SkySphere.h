#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\InputLayout.h"

class SkySphere : public InputLayout
{
public:
	SkySphere();
	~SkySphere();
	void					Initialize();
	virtual void			Update(FLOAT delta);
	void					SetBuffer();

private:
	vector<IL_Pos>			mVertexData;
	vector<UINT>			mIndexData;

	virtual void			InitializeData();
	virtual void			InitializeBuffer();
};