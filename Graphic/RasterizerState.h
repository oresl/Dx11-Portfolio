#pragma once
#include "..\Common\Pch.h"
#include "Direct3D.h"

typedef enum RASTERIZER_STATE_ENUM
{
	RS_Solid = 0,
	RS_WireFrame,
	RS_WireFrame_CullNone,
	RS_CullNone,
	RS_WireFrame_CullBack,
	RS_CullBack,
	RS_CullFront,
	RS_CW,
	RS_CCW,
	RS_Default,
	RASTERIZER_STATE_COUNT,
} RS_Type;

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();
	void					Initialize();
	void					SetState(RS_Type type);

private:
	ID3D11RasterizerState**	mState;

};