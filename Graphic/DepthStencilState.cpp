#include "DepthStencilState.h"

// public
DepthStencilState::DepthStencilState()
{
	mStencilRef = 0;
	mState		= NULL;
}

DepthStencilState::~DepthStencilState()
{
	for (int i = 0; i < (int)DEPTH_STENCIL_STATE_COUNT - 1; i++)
	{
		SafeReleaseCom(mState[i]);
	}
}

void DepthStencilState::Initialize()
{
	mState = new ID3D11DepthStencilState*[(int)DEPTH_STENCIL_STATE_COUNT - 1];

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Depth Disable
	dsDesc.DepthEnable		= FALSE;
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= TRUE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	/*
	StencilFailOp,	StencilDepthFailOp,	StencilPassOp,	StencilFunc
	*/
	D3D11_DEPTH_STENCILOP_DESC frontStencilOp =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };

	D3D11_DEPTH_STENCILOP_DESC backStencilOp =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_DECR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };

	dsDesc.FrontFace = frontStencilOp;
	dsDesc.BackFace = backStencilOp;

	D3D.GetDevice()->CreateDepthStencilState(&dsDesc, &mState[(int)DS_DepthDisable]);

	mStencilRef = 1;
}

void DepthStencilState::SetState(DS_Type type)
{
	if (type != DS_Default)
		D3D.GetContext()->OMSetDepthStencilState(mState[(int)type], mStencilRef);
	else
		D3D.GetContext()->OMSetDepthStencilState(NULL, (UINT)0);
}