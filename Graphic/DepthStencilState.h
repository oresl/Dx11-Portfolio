#pragma once
#include "..\Common\Pch.h"
#include "Direct3D.h"

typedef enum DEPTH_STENCIL_STATE_ENUM
{
	DS_DepthDisable = 0,
	DS_Default,
	DEPTH_STENCIL_STATE_COUNT,
} DS_Type;

class DepthStencilState
{
public:
	DepthStencilState();
	~DepthStencilState();
	void						Initialize();
	void						SetState(DS_Type type);

private:
	UINT						mStencilRef;
	ID3D11DepthStencilState**	mState;

};