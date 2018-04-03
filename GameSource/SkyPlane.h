#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\RenderResource\InputLayout.h"

class SkyPlane : public InputLayout
{
public:
	SkyPlane();
	~SkyPlane();
	void					Initialize();
	virtual void			Update(FLOAT delta);
	void					SetBuffer();

private:
	struct BufferData
	{
		FLOAT Translation;
		FLOAT Scale;
		FLOAT Brightness;
		FLOAT Pad;
	};

	vector<IL_PosTex>		mVertexData;
	vector<UINT>			mIndexData;
	
	ID3D11Buffer*			mBuffer;
	FLOAT					mTranslation;

	virtual void			InitializeData();
	virtual void			InitializeBuffer();
};