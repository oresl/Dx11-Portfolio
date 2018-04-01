#pragma once
#include "..\Common\Pch.h"
#include "..\RenderResource\VertexLayout.h"

class Cube : public VertexLayout
{
public:
	Cube();
	~Cube();
	void					Initialize();

private:
	UINT					mVertexCount;
	UINT					mIndexCount;
	vector<VL_PosTexNor>	mVertexData;
	vector<UINT>			mIndexData;
	

	virtual void InitializeData();
	virtual void InitializeBuffer();
};

