#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\InputLayout.h"

class Cube : public InputLayout
{
public:
	Cube();
	~Cube();
	void					Initialize();
	virtual void			Update(FLOAT delta);
	void					SetBuffer();

private:
	vector<IL_PosTexNor>	mVertexData;
	vector<UINT>			mIndexData;

	virtual void			InitializeData();
	virtual void			InitializeBuffer();
};

