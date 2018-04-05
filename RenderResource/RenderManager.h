#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "..\Graphic\RasterizerState.h"
#include "..\Graphic\BlendState.h"
#include "..\Graphic\DepthStencilState.h"
#include "Shader.h"
#include "InputLayout.h"

typedef enum TEXTURE_EXTENSION_ENUM
{
	TE_DDS = 0,
	TE_PNG,
	TE_JPG,
	TE_GIF,
	TE_BMP,
	TEXTURE_EXTENSION_COUNT,
} TE_Type;

class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	void Initialize();
	void AddShader(wstring shaderName, const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount, bool tessellated = false);
	void AddTexture(wstring textureName, TE_Type ext);
	void AddTextureArray(wstring arrayName, vector<wstring> textureNames, TE_Type ext);
	void SetShader(wstring shaderName);
	void SetTexture(wstring textureName, UINT startSlot);
	void SetTexture(ID3D11ShaderResourceView* srv, UINT startSlot);
	void SetTextureArray(wstring arrayName, UINT startSlot);
	void SetRasterizerState(RS_Type type);
	void SetBlendState(BS_Type type);
	void SetDepthStencilState(DS_Type type);
	void SetIAParameterAndDraw(InputLayout* object);

private:
	map<wstring, Shader*>					mShaderList;
	map<wstring, ID3D11ShaderResourceView*> mTextureList;
	map<wstring, ID3D11ShaderResourceView*> mTextureArrayList;
	RasterizerState*						mRasterizerState;
	BlendState*								mBlendState;
	DepthStencilState*						mDepthStencilState;

};

