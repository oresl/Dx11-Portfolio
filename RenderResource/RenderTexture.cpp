#include "RenderTexture.h"

// public
RenderTexture::RenderTexture()
{
	mRenderTargetTexture	= NULL;
	mRenderTargetView		= NULL;
	mShaderResourceView		= NULL;
	mDepthStencilTexture	= NULL;
	mDepthStencilView		= NULL;
}

RenderTexture::~RenderTexture()
{
	SafeReleaseCom(mDepthStencilView);
	SafeReleaseCom(mDepthStencilTexture);
	SafeReleaseCom(mShaderResourceView);
	SafeReleaseCom(mRenderTargetView);
	SafeReleaseCom(mRenderTargetTexture);
}

void RenderTexture::Initialize(UINT textureWidth, UINT textureHeight)
{
	mWidth	= textureWidth;
	mHeight = textureHeight;

	// ·»´õÅ¸°Ù »ý¼º (·»´õÅ¸°Ù ºä + Depth-Stencil ºä)
	// ·£´õÅ¸°Ù ºä¿¡ ¿¬°áÇÒ texture»ý¼º
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width				= mWidth;
	texDesc.Height				= mHeight;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count	= 1;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;

	HRESULT hr = D3D.GetDevice()->CreateTexture2D(&texDesc, NULL, &mRenderTargetTexture);
	assert(SUCCEEDED(hr));

	// ·»´õÅ¸°Ù ºä»ý¼º
	D3D11_RENDER_TARGET_VIEW_DESC rtViewDesc;
	ZeroMemory(&rtViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtViewDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	rtViewDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtViewDesc.Texture2D.MipSlice = 0;

	hr = D3D.GetDevice()->CreateRenderTargetView(mRenderTargetTexture, &rtViewDesc, &mRenderTargetView);
	assert(SUCCEEDED(hr));

	// SRV»ý¼º
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format						= DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	srvDesc.Texture2D.MipLevels			= 1;

	hr = D3D.GetDevice()->CreateShaderResourceView(mRenderTargetTexture, &srvDesc, &mShaderResourceView);
	assert(SUCCEEDED(hr));

	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width				= mWidth;
	texDesc.Height				= mHeight;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;

	hr = D3D.GetDevice()->CreateTexture2D(&texDesc, NULL, &mDepthStencilTexture);
	assert(SUCCEEDED(hr));
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	ZeroMemory(&dsViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Texture2D.MipSlice	= 0;

	hr = D3D.GetDevice()->CreateDepthStencilView(mDepthStencilTexture, &dsViewDesc, &mDepthStencilView);
	assert(SUCCEEDED(hr));

	// ºäÆ÷Æ®
	mViewport.Width		= (FLOAT)mWidth;
	mViewport.Height	= (FLOAT)mHeight;
	mViewport.MinDepth	= 0.0F;
	mViewport.MaxDepth	= 1.0F;
	mViewport.TopLeftX	= 0.0F;
	mViewport.TopLeftY	= 0.0F;
	
	// 3D ÇÁ·ÎÁ§¼Ç Çà·Ä
	D3DXMatrixPerspectiveFovLH
	(
		&mProjection,
		((FLOAT)D3DX_PI / 4.0f),
		((FLOAT)mWidth / (FLOAT)mHeight),
		gCFEngine.Near,
		gCFEngine.Depth
	);

	// 2D ÇÁ·ÎÁ§¼Ç Çà·Ä
	D3DXMatrixOrthoLH
	(
		&mOrthographic,
		(FLOAT)mWidth,
		(FLOAT)mHeight,
		gCFEngine.Near,
		gCFEngine.Depth
	);
}

void RenderTexture::SetRenderTarget()
{
	D3D.GetContext()->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	D3D.GetContext()->RSSetViewports(1, &mViewport);
}

void RenderTexture::Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	FLOAT clearcolor[4] = { r, g, b, a };

	D3D.GetContext()->ClearRenderTargetView(mRenderTargetView, clearcolor);
	D3D.GetContext()->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0F, 0);
}

// 2018.03.21 ½ºÅ©¸°¼¦ ¿ëÀ¸·Î Ãß°¡
void RenderTexture::SaveTexture(wstring filename)
{
	HRESULT hr = D3DX11SaveTextureToFile(
		D3D.GetContext(),
		mRenderTargetTexture,
		D3DX11_IFF_PNG,
		filename.c_str()
	);
	assert(SUCCEEDED(hr));
}

ID3D11ShaderResourceView* RenderTexture::GetShaderResourceView()	{ return mShaderResourceView; }
D3DXMATRIX RenderTexture::GetProjection()							{ return mProjection; }
D3DXMATRIX RenderTexture::GetOrthographic()							{ return mOrthographic; }