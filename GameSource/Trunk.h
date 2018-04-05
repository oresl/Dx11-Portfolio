#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\InputLayout.h"
#include "Terrain.h"

class Trunk : public InputLayout
{
public:
	Trunk();
	~Trunk();
	void					Initialize(Terrain* terrain, FLOAT scale);
	void					Update(Frustum* frustum);
	void					SetBuffer();
	D3DXVECTOR3				GetInstancePos(int index);

private:
	vector<IL_PosTex>		mVertexData;
	vector<UINT>			mIndexData;
	vector<IL_PosTexInst>	mInstanceData;
	vector<D3DXVECTOR3>		mInstancePos;

	virtual void			InitializeData(Terrain* terrain, FLOAT scale);
	virtual void			InitializeBuffer();
};

