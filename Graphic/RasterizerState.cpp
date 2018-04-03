#include "RasterizerState.h"

// public
RasterizerState::RasterizerState()
{
	mState = NULL;
}

RasterizerState::~RasterizerState()
{
	for (int i = 0; i < (int)RS_Type::RASTERIZER_STATE_COUNT - 1; i++)
	{
		SafeReleaseCom(mState[i]);
	}
}

void RasterizerState::Initialize()
{
	// Default�� �ʿ�x
	mState = new ID3D11RasterizerState*[(int)RASTERIZER_STATE_COUNT - 1];

	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));

	// ** �Ʒ� �� �Ӽ��� ������ �������� ��� 0 (NULL, FALSE)�� �⺻��
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.CullMode = D3D11_CULL_BACK;

	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	HRESULT hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_WireFrame]);
	assert(SUCCEEDED(hr));

	rsDesc.CullMode = D3D11_CULL_NONE;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_WireFrame_CullNone]);
	assert(SUCCEEDED(hr));

	rsDesc.CullMode = D3D11_CULL_BACK;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_WireFrame_CullBack]);
	assert(SUCCEEDED(hr));

	rsDesc.FillMode = D3D11_FILL_SOLID;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_Solid]);
	assert(SUCCEEDED(hr));

	rsDesc.CullMode = D3D11_CULL_NONE;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_CullNone]);
	assert(SUCCEEDED(hr));

	rsDesc.CullMode = D3D11_CULL_FRONT;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_CullFront]);
	assert(SUCCEEDED(hr));

	rsDesc.CullMode = D3D11_CULL_BACK;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_CullBack]);
	assert(SUCCEEDED(hr));

	// (ī�޶� ��������)�ð����			clock wise
	rsDesc.FrontCounterClockwise = FALSE;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_CW]);
	assert(SUCCEEDED(hr));

	// (ī�޶� ��������)�ݽð����		counter clock wise
	rsDesc.FrontCounterClockwise = TRUE;
	hr = D3D.GetDevice()->CreateRasterizerState(&rsDesc, &mState[(int)RS_CCW]);
	assert(SUCCEEDED(hr));
}

void RasterizerState::SetState(RS_Type type)
{
	if (type != RS_Default)
		D3D.GetContext()->RSSetState(mState[(int)type]);
	else
		D3D.GetContext()->RSSetState(NULL);
}