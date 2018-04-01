#pragma once
#include "..\Common\Pch.h"

class Direct3D
{
public:
	Direct3D();
	~Direct3D();
	void Initialize();
	void Clear(FLOAT r = 0.2F, FLOAT g = 0.2F, FLOAT b = 0.2F, FLOAT a = 1.0F);
	void Present();

	// Get, Set
	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetContext();
	void					SetDefaultRenderTarget();
	void					SetDefaultViewport();

	Singleton_h(Direct3D)

private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	UINT					m4xMsaaQuality;
	IDXGISwapChain*			mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D*		mDepthStencilTexture;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT			mViewport;

	void					InitializeDeviceAndContext();
	void					InitializeMSAA();
	void					InitializeSwapChain();
	void					InitializeRenderTargetView();
	void					InitializeDepthStencilView();
	void					InitializeBindView();
	void					InitializeViewport();

};

