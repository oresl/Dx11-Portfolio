#include "BlendState.h"

// public
BlendState::BlendState()
{
	mBlendFactor[0] = 0.0F;
	mBlendFactor[1] = 0.0F;
	mBlendFactor[2] = 0.0F;
	mBlendFactor[3] = 0.0F;
	mSampleMask		= 0;
	mState			= NULL;
}

BlendState::~BlendState()
{
	for (int i = 0; i < (int)BLEND_STATE_COUNT - 1; i++)
	{
		SafeReleaseCom(mState[i]);
	}
}

void BlendState::Initialize()
{
	// Default는 필요x
	mState = new ID3D11BlendState*[(int)BLEND_STATE_COUNT - 1];

	D3D11_BLEND_DESC bsDesc;
	ZeroMemory(&bsDesc, sizeof(D3D11_BLEND_DESC));

	bsDesc.IndependentBlendEnable	= FALSE;	// FALSE면 모든 렌더타겟이 RenderTarget[0]의 속성을 사용

	/*
	SrcBlend				: 소스 색상
	DestBlend				: 대상 색상
	BlendOp					: 색상 연산자 설정
	SrcBlendAlpha			: 소스 알파
	DestBlendAlpha			: 대상 알파
	BlendOpAlpha			: 알파 연산자 설정
	RenderTargetWriteMask	: 색상 (r, g, b, a) 마스크 선택
	*/

	//for (int i = 0; i < ARRAYSIZE(bsDesc.RenderTarget); i++)
	//{
	//	bsDesc.RenderTarget[i].BlendEnable				= TRUE;
	//	bsDesc.RenderTarget[i].SrcBlend					= D3D11_BLEND_SRC_ALPHA;
	//	bsDesc.RenderTarget[i].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	//	bsDesc.RenderTarget[i].BlendOp					= D3D11_BLEND_OP_ADD;
	//	bsDesc.RenderTarget[i].SrcBlendAlpha			= D3D11_BLEND_ONE;
	//	bsDesc.RenderTarget[i].DestBlendAlpha			= D3D11_BLEND_ZERO;
	//	bsDesc.RenderTarget[i].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	//	bsDesc.RenderTarget[i].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	//}
	
	// Transparency
	bsDesc.AlphaToCoverageEnable					= FALSE;
	bsDesc.RenderTarget[0].BlendEnable				= TRUE;
	bsDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_SRC_ALPHA;
	bsDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	bsDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D.GetDevice()->CreateBlendState(&bsDesc, &mState[(int)BS_Transparency]);
	
	// Alpha
	// Transparency
	bsDesc.AlphaToCoverageEnable					= TRUE;
	bsDesc.RenderTarget[0].BlendEnable				= TRUE;
	bsDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	bsDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	
	D3D.GetDevice()->CreateBlendState(&bsDesc, &mState[(int)BS_Alpha]);

	// SkyPlane
	bsDesc.AlphaToCoverageEnable					= FALSE;
	bsDesc.RenderTarget[0].BlendEnable				= TRUE;
	bsDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	bsDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D.GetDevice()->CreateBlendState(&bsDesc, &mState[(int)BS_SkyPlane]);

	mBlendFactor[0] = 0.0F;
	mBlendFactor[1] = 0.0F;
	mBlendFactor[2] = 0.0F;
	mBlendFactor[3] = 0.0F;

	mSampleMask = 0xffffffff;
}

void BlendState::SetState(BS_Type type)
{
	if (type != BS_Default)
		D3D.GetContext()->OMSetBlendState(mState[(int)type], mBlendFactor, mSampleMask);
	else
		D3D.GetContext()->OMSetBlendState(NULL, NULL, mSampleMask);
}