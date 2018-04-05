#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\InputLayout.h"
#include "Trunk.h"

class Leaf : public InputLayout
{
public:
	Leaf();
	~Leaf();
	void					Initialize(Trunk* trunk, FLOAT scale);
	void					Update(Frustum* frustum);
	void					SetBuffer();

private:
	vector<IL_PosTex>		mVertexData;
	vector<UINT>			mIndexData;
	vector<IL_PosTexInst>	mInstanceData;
	vector<D3DXVECTOR3>		mInstancePos;

	virtual void			InitializeData(Trunk* trunk, FLOAT scale);
	virtual void			InitializeBuffer();
};
