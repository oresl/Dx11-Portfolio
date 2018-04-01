#include "Shader.h"

// public
Shader::Shader()
{
	mShaderName		= L"";
	mVertexShader	= NULL;
	mHullShader		= NULL;
	mDomainShader	= NULL;
	mPixelShader	= NULL;
	mInputLayout	= NULL;
	mShaderBlob		= NULL;
}

Shader::~Shader()
{
	SafeReleaseCom(mShaderBlob);
	SafeReleaseCom(mInputLayout);
	SafeReleaseCom(mPixelShader);
	SafeReleaseCom(mDomainShader);
	SafeReleaseCom(mHullShader);
	SafeReleaseCom(mVertexShader);
	mShaderName.clear();
}

void Shader::Initialize(wstring shaderName, const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount, bool tessellated)
{
	mShaderName = shaderName;

	if (tessellated)
	{
		InitializeShader(IS_Hull);
		InitializeShader(IS_Domain);
	}

	// ** InputLayout 생성시 Vertex Blob이 필요하기 때문에, Vertex Shader버퍼를 가장 마지막에 생성
	InitializeShader(IS_Vertex);
	InitializeShader(IS_Pixel);

	InitializeInputLayout(elementDesc, elementCount);
}

// private
void Shader::InitializeShader(IS_Type type)
{
	// 컴파일
	wstring path = L"GameSource\\" + mShaderName + L".fx";

	char* entry;
	char* profile;

	switch (type)
	{
	case IS_Vertex: entry = "VS"; profile = "vs_5_0"; break;
	case IS_Hull:	entry = "HS"; profile = "hs_5_0"; break;
	case IS_Domain: entry = "DS"; profile = "ds_5_0"; break;
	case IS_Pixel:	entry = "PS"; profile = "ps_5_0"; break;
	}

	ID3D10Blob* error = NULL;
	HRESULT hr = D3DX11CompileFromFile
	(
		path.c_str(),
		NULL,
		NULL,
		entry,
		profile,
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&mShaderBlob,
		&error,
		NULL
	);
	if (FAILED(hr))
	{
		// **error 해제는 WriteShaderLog함수에서 함
		WriteShaderLog(error);
		return;
	}

	// 각 쉐이더 버퍼생성
	switch (type)
	{
	case IS_Vertex: hr = D3D.GetDevice()->CreateVertexShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), NULL, &mVertexShader);
	case IS_Hull:	hr = D3D.GetDevice()->CreateHullShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), NULL, &mHullShader);
	case IS_Domain: hr = D3D.GetDevice()->CreateDomainShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), NULL, &mDomainShader);
	case IS_Pixel:	hr = D3D.GetDevice()->CreatePixelShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), NULL, &mPixelShader);
	}
}

void Shader::InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount)
{
	HRESULT hr = D3D.GetDevice()->CreateInputLayout
	(
		elementDesc,
		elementCount,
		mShaderBlob->GetBufferPointer(),
		mShaderBlob->GetBufferSize(),
		&mInputLayout
	);
	assert(SUCCEEDED(hr));

	SafeReleaseCom(mShaderBlob);
}