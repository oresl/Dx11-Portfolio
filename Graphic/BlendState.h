#pragma once
#include "..\Common\Pch.h"
#include "Direct3D.h"

typedef enum BLEND_STATE_ENUM
{
	BS_Transparency = 0,
	BS_Alpha,
	BS_SkyPlane,
	BS_Default,
	BLEND_STATE_COUNT,
} BS_Type;

class BlendState
{
public:
	BlendState();
	~BlendState();
	void					Initialize();
	void					SetState(BS_Type type);

private:
	FLOAT					mBlendFactor[4];
	UINT					mSampleMask;

	ID3D11BlendState**		mState;

};