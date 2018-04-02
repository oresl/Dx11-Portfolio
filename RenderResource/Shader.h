#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"

class Shader
{
public:
	Shader();
	~Shader();
	void Initialize(wstring shaderName, const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount, bool tessellated = false);
	void SetShader();

private:
	typedef enum INITIALIZE_SHADER_ENUM
	{
		IS_Vertex = 0,
		IS_Hull,
		IS_Domain,
		IS_Pixel,
		INITIALIZE_SHADER_COUNT,
	} IS_Type;
	
	wstring				mName;
	bool				mTessellated;
	ID3D11VertexShader* mVS;
	ID3D11HullShader*	mHS;
	ID3D11DomainShader* mDS;
	ID3D11PixelShader*	mPS;
	ID3D10Blob*			mShaderBlob;
	ID3D11InputLayout*	mInputLayout;

	void InitializeShader(IS_Type type);
	void InitializeInputLayout(const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount);
};