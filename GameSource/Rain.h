#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\Camera.h"
#include "..\RenderResource\Frustum.h"
#include "..\RenderResource\InputLayout.h"

class Rain : public InputLayout
{
public:
	Rain();
	~Rain();
	void					Initialize();
	virtual void			Update(FLOAT delta, Camera* cam, Frustum* frustum);
	void					SetBuffer();

private:
	vector<IL_Pos>			mVertexData;
	vector<UINT>			mIndexData;
	vector<IL_Instance>		mInstanceData;
	vector<D3DXVECTOR3>		mInstancePos;

	virtual void			InitializeData();
	virtual void			InitializeBuffer();
};