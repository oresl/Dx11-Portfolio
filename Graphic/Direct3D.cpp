#include "Direct3D.h"

// static
Singleton_cpp(Direct3D)

// pubilc
Direct3D::Direct3D()
{
	mDevice					= NULL;
	mDeviceContext			= NULL;
	m4xMsaaQuality			= 0;
	mSwapChain				= NULL;
	mRenderTargetView		= NULL;
	mDepthStencilTexture	= NULL;
	mDepthStencilView		= NULL;
	
	ZeroMemory(&mViewport, sizeof(D3D11_VIEWPORT));
}

Direct3D::~Direct3D()
{
	SafeReleaseCom(mDepthStencilView);
	SafeReleaseCom(mDepthStencilTexture);
	SafeReleaseCom(mRenderTargetView);
	SafeReleaseCom(mSwapChain);
	SafeReleaseCom(mDeviceContext);
	SafeReleaseCom(mDevice);
}

void Direct3D::Initialize()
{
	InitializeDeviceAndContext();
	InitializeMSAA();
	InitializeSwapChain();
	InitializeRenderTargetView();
	InitializeDepthStencilView();
	InitializeBindView();
	InitializeViewport();
}

void Direct3D::Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	FLOAT clearcolor[4] = { r, g, b, a };

	mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearcolor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0F, 0);
}

void Direct3D::Present()
{
	if (gCFEngine.VsyncEnabled)
		mSwapChain->Present(1, 0);
	else
		mSwapChain->Present(0, 0);
}

// Get, Set
ID3D11Device* Direct3D::GetDevice()			{ return mDevice; }
ID3D11DeviceContext* Direct3D::GetContext()	{ return mDeviceContext; }

void Direct3D::SetDefaultRenderTarget()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetViewports(1, &mViewport);
}

void Direct3D::SetDefaultViewport()
{
	mDeviceContext->RSSetViewports(1, &mViewport);
}

// private
void Direct3D::InitializeDeviceAndContext()
{
	// 배열순서대로 GPU가 지원하는지 확인후 가장먼저 성공하는 featurelevel로 초기화
	D3D_FEATURE_LEVEL featurelevel[6] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT hr = D3D11CreateDevice
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featurelevel,
		ARRAYSIZE(featurelevel),
		D3D11_SDK_VERSION,
		&mDevice,
		NULL,
		&mDeviceContext
	);
	assert(SUCCEEDED(hr));
}

void Direct3D::InitializeMSAA()
{
	if (gCFEngine.MSAAEnabled)
	{
		HRESULT hr = mDevice->CheckMultisampleQualityLevels
		(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			4,
			&m4xMsaaQuality
		);
		assert(m4xMsaaQuality > 0);
	}
}

void Direct3D::InitializeSwapChain()
{
	IDXGIDevice* device = NULL;
	HRESULT hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&device);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter = NULL;
	hr = device->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter);
	assert(SUCCEEDED(hr));

	DXGI_ADAPTER_DESC adapterDesc;
	ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));
	hr = adapter->GetDesc(&adapterDesc);
	assert(SUCCEEDED(hr));

	// GPU 정보 로그에 기록 (GPU 이름, 메모리크기)
	wstring gpuInfo = L"GPU Description : " + wstring(adapterDesc.Description);
	WriteLog(gpuInfo, L_Information);

	WCHAR buffer[5];
	_itow_s((int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024), buffer, 10);
	gpuInfo = L"";
	gpuInfo = L"GPU Mem Size : " + wstring(buffer) + L"MB";
	WriteLog(gpuInfo, L_Information);

	IDXGIOutput* output = NULL;
	hr = adapter->EnumOutputs(0, &output);
	assert(SUCCEEDED(hr));

	// output으로부터 설정가능한 수를 얻어옴
	UINT numModes;
	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	assert(SUCCEEDED(hr));
	
	// 동일한 함수를 호출하는데 설정을 얻어옴
	DXGI_MODE_DESC* modeDesc = new DXGI_MODE_DESC[numModes];
	hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modeDesc);
	assert(SUCCEEDED(hr));

	// 설정으로부터 게임의 해상도와 맞는 값을 설정
	UINT numerator		= 0;
	UINT denominator	= 0;

	for (UINT i = 0; i < numModes; i++)
	{
		if (modeDesc[i].Width == gCFEngine.Width)
		{
			if (modeDesc[i].Height == gCFEngine.Height)
			{
				numerator	= modeDesc[i].RefreshRate.Numerator;
				denominator = modeDesc[i].RefreshRate.Denominator;
			}
		}
	}

	IDXGIFactory* factory = NULL;
	hr = adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);
	assert(SUCCEEDED(hr));

	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapchainDesc.BufferDesc.Width						= gCFEngine.Width;
	swapchainDesc.BufferDesc.Height						= gCFEngine.Height;

	if (gCFEngine.VsyncEnabled)
	{
		swapchainDesc.BufferDesc.RefreshRate.Numerator		= numerator;
		swapchainDesc.BufferDesc.RefreshRate.Denominator	= denominator;
	}
	else
	{
		swapchainDesc.BufferDesc.RefreshRate.Numerator		= 0;
		swapchainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	}

	swapchainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	if (gCFEngine.MSAAEnabled)
	{
		swapchainDesc.SampleDesc.Count		= 4;
		swapchainDesc.SampleDesc.Quality	= m4xMsaaQuality - 1;
	}
	else
	{
		swapchainDesc.SampleDesc.Count		= 1;
		swapchainDesc.SampleDesc.Quality	= 0;
	}

	swapchainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount	= 1;
	swapchainDesc.OutputWindow	= gCFEngine.Hwnd;
	swapchainDesc.Windowed		= gCFEngine.Windowed;
	swapchainDesc.SwapEffect	= DXGI_SWAP_EFFECT_DISCARD;
	swapchainDesc.Flags			= 0;

	hr = factory->CreateSwapChain(mDevice, &swapchainDesc, &mSwapChain);
	assert(SUCCEEDED(hr));

	SafeReleaseCom(factory);
	SafeReleaseCom(adapter);
	SafeReleaseCom(device);
}

void Direct3D::InitializeRenderTargetView()
{
	ID3D11Texture2D* backbuffer = NULL;
	HRESULT hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);
	assert(SUCCEEDED(hr));

	hr = mDevice->CreateRenderTargetView(backbuffer, NULL, &mRenderTargetView);
	assert(SUCCEEDED(hr));

	SafeReleaseCom(backbuffer);
}

void Direct3D::InitializeDepthStencilView()
{
	D3D11_TEXTURE2D_DESC depthstencilDesc;
	ZeroMemory(&depthstencilDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthstencilDesc.Width		= gCFEngine.Width;
	depthstencilDesc.Height		= gCFEngine.Height;
	depthstencilDesc.MipLevels	= 1;
	depthstencilDesc.ArraySize	= 1;
	depthstencilDesc.Format		= DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (gCFEngine.MSAAEnabled)
	{
		depthstencilDesc.SampleDesc.Count	= 4;
		depthstencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		depthstencilDesc.SampleDesc.Count	= 1;
		depthstencilDesc.SampleDesc.Quality = 0;
	}

	depthstencilDesc.Usage			= D3D11_USAGE_DEFAULT;
	depthstencilDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	depthstencilDesc.CPUAccessFlags = 0;
	depthstencilDesc.MiscFlags		= 0;

	HRESULT hr = mDevice->CreateTexture2D(&depthstencilDesc, NULL, &mDepthStencilTexture);
	assert(SUCCEEDED(hr));

	hr = mDevice->CreateDepthStencilView(mDepthStencilTexture, NULL, &mDepthStencilView);
	assert(SUCCEEDED(hr));}

void Direct3D::InitializeBindView()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void Direct3D::InitializeViewport()
{
	mViewport.TopLeftX	= 0.0F;
	mViewport.TopLeftY	= 0.0F;
	mViewport.Width		= (FLOAT)gCFEngine.Width;
	mViewport.Height	= (FLOAT)gCFEngine.Height;
	mViewport.MinDepth	= 0.0F;
	mViewport.MaxDepth	= 1.0F;

	mDeviceContext->RSSetViewports(1, &mViewport);
}