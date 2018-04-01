#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"

class Shader
{
public:
	Shader();
	~Shader();
	void Initialize(wstring shaderName, const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount, bool tessellated = false);

private:
	typedef enum INITIALIZE_SHADER_ENUM
	{
		IS_Vertex = 0,
		IS_Hull,
		IS_Domain,
		IS_Pixel,
		INITIALIZE_SHADER_COUNT,
	} IS_Type;

	wstring				mShaderName;
	ID3D11VertexShader* mVertexShader;
	ID3D11HullShader*	mHullShader;
	ID3D11DomainShader* mDomainShader;
	ID3D11PixelShader*	mPixelShader;
	ID3D11InputLayout*	mInputLayout;
	ID3D10Blob*			mShaderBlob;

	void InitializeShader(IS_Type type);
	void InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount);

};