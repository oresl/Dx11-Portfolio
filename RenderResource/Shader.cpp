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

	// ** InputLayout ������ Vertex Blob�� �ʿ��ϱ� ������, Vertex Shader���۸� ���� �������� ����
	InitializeShader(IS_Vertex);
	InitializeShader(IS_Pixel);

	InitializeInputLayout(elementDesc, elementCount);
}

// private
void Shader::InitializeShader(IS_Type type)
{
	// ������
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
		// **error ������ WriteShaderLog�Լ����� ��
		WriteShaderLog(error);
		return;
	}

	// �� ���̴� ���ۻ���
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