#include "RenderManager.h"

// static
Singleton_cpp(RenderManager)

// public
RenderManager::RenderManager()
{
	mShaderList.clear();
}

RenderManager::~RenderManager()
{
	for (auto shader : mShaderList)
	{
		SafeDeletePtr(shader.second);
	}
	mShaderList.clear();
}

void RenderManager::AddShader(wstring shaderName, const D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount, bool tessellated)
{
	auto shader = mShaderList.find(shaderName);
	
	if (shader != mShaderList.end())
		return;

	Shader* newShader = new Shader;
	newShader->Initialize(shaderName, elementDesc, elementCount, tessellated);

	mShaderList.insert(make_pair(shaderName, newShader));
}

void RenderManager::SetShader(wstring shaderName)
{
	auto shader = mShaderList.find(shaderName);

	if (shader == mShaderList.end())
		return;

	shader->second->SetShader();
}

void RenderManager::SetIAParameter(InputLayout* object)
{
	// 테셀레이션 확인
	D3D_PRIMITIVE_TOPOLOGY topology;

	if (object->CheckTessellated())
		topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	else
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 인스턴싱 확인
	if (object->CheckInstanced())
	{
		UINT strides[2]			= { object->GetInputLayoutSize(), object->GetInputLayoutInstanceSize() };
		UINT offsets[2]			= { 0, 0 };
		ID3D11Buffer* buff[2]	= { object->GetVertexBuffer(), object->GetInstanceBuffer() };
		ID3D11Buffer* indexBuff = object->GetIndexBuffer();

		D3D.GetContext()->IASetVertexBuffers(0, 2, buff, strides, offsets);
		D3D.GetContext()->IASetIndexBuffer(indexBuff, DXGI_FORMAT_R32_UINT, 0);

		D3D.GetContext()->IASetPrimitiveTopology(topology);
		D3D.GetContext()->DrawIndexedInstanced(object->GetIndexCount(), object->GetDrawCount(), 0, 0, 0);
	}
	else
	{
		UINT strides				= object->GetInputLayoutSize();
		UINT offsets				= 0;
		ID3D11Buffer* vertexBuff	= object->GetVertexBuffer();
		ID3D11Buffer* indexBuff		= object->GetIndexBuffer();

		D3D.GetContext()->IASetVertexBuffers(0, 1, &vertexBuff, &strides, &offsets);
		D3D.GetContext()->IASetIndexBuffer(indexBuff, DXGI_FORMAT_R32_UINT, 0);

		D3D.GetContext()->IASetPrimitiveTopology(topology);
		D3D.GetContext()->DrawIndexed(object->GetIndexCount(), 0, 0);
	}
}