#pragma once
#include "Shader.h"
#include "VertexLayout.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	// Resource Add
	void AddShader(wstring shaderName, const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount, bool tessellated = false);

	// PipeLine
	void SetShader();
	void SetIABuffer(VertexLayout* object);
	void SetPrimitiveToplogyAndDraw();

private:
	map<wstring, Shader*> mShaderList;

};

