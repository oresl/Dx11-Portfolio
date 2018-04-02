#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"
#include "Shader.h"
#include "InputLayout.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	void AddShader(wstring shaderName, const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount, bool tessellated = false);
	void SetShader(wstring shaderName);
	void SetIAParameter(InputLayout* object);

	Singleton_h(RenderManager)

private:
	map<wstring, Shader*> mShaderList;

};

