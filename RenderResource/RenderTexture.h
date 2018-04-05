#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();
	void						Initialize(UINT textureWidth = gCFEngine.Width, UINT textureHeight = gCFEngine.Height);
	void						SetRenderTarget();
	void						Clear(FLOAT r = 0.0F, FLOAT g = 0.0F, FLOAT b = 0.0F, FLOAT a = 1.0F);
	void						SaveTexture(wstring filename);
	ID3D11ShaderResourceView*	GetShaderResourceView();
	D3DXMATRIX					GetProjection();
	D3DXMATRIX					GetOrthographic();

private:
	UINT						mWidth;
	UINT						mHeight;

	ID3D11Texture2D*			mRenderTargetTexture;
	ID3D11RenderTargetView*		mRenderTargetView;
	ID3D11ShaderResourceView*	mShaderResourceView;

	ID3D11Texture2D*			mDepthStencilTexture;
	ID3D11DepthStencilView*		mDepthStencilView;
	D3D11_VIEWPORT				mViewport;

	D3DXMATRIX					mProjection;
	D3DXMATRIX					mOrthographic;

};