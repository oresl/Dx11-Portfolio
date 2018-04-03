#include "RenderManager.h"

// static
Singleton_cpp(RenderManager)

// public
RenderManager::RenderManager()
{
	mShaderList.clear();
	mTextureList.clear();
	mTextureArrayList.clear();
}

RenderManager::~RenderManager()
{
	for (auto shader : mShaderList)
	{
		SafeDeletePtr(shader.second);
	}
	mShaderList.clear();

	for (auto texture : mTextureList)
	{
		SafeReleaseCom(texture.second);
	}
	mTextureList.clear();

	for (auto textureArray : mTextureArrayList)
	{
		SafeReleaseCom(textureArray.second);
	}
	mTextureArrayList.clear();
}

void RenderManager::Initialize()
{
	mRasterizerState = new RasterizerState;
	mRasterizerState->Initialize();

	mBlendState = new BlendState;
	mBlendState->Initialize();

	mDepthStencilState = new DepthStencilState;
	mDepthStencilState->Initialize();
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

void RenderManager::AddTexture(wstring textureName, EXT_Type ext)
{
	auto texture = mTextureList.find(textureName);
	if (texture != mTextureList.end())
		return;

	wstring extensionName;
	switch (ext)
	{
	case TE_DDS: extensionName = L"dds"; break;
	case TE_PNG: extensionName = L"png"; break;
	case TE_JPG: extensionName = L"jpg"; break;
	case TE_GIF: extensionName = L"gif"; break;
	case TE_BMP: extensionName = L"bmp"; break;
	}

	ID3D11ShaderResourceView* newTexture;
	CreateShaderResourceView(D3D.GetDevice(), textureName, extensionName, &newTexture);

	mTextureList.insert(make_pair(textureName, newTexture));
}

void RenderManager::AddTextureArray(wstring arrayName, vector<wstring> textureNames, EXT_Type ext)
{
	auto textureArray = mTextureArrayList.find(arrayName);
	if (textureArray != mTextureArrayList.end())
		return;

	wstring extensionName;
	switch (ext)
	{
	case TE_DDS: extensionName = L"dds"; break;
	case TE_PNG: extensionName = L"png"; break;
	case TE_JPG: extensionName = L"jpg"; break;
	case TE_GIF: extensionName = L"gif"; break;
	case TE_BMP: extensionName = L"bmp"; break;
	}

	ID3D11ShaderResourceView* newTexture;
	CreateShaderResourceViewArray(D3D.GetDevice(), D3D.GetContext(), textureNames, extensionName, &newTexture);

	mTextureList.insert(make_pair(arrayName, newTexture));
}

void RenderManager::SetShader(wstring shaderName)
{
	auto shader = mShaderList.find(shaderName);

	if (shader == mShaderList.end())
	{
		WriteLog(L"No shader in List", L_Warning);
		return;
	}

	shader->second->SetShader();
}

void RenderManager::SetTexture(wstring textureName, UINT startSlot)
{
	auto texture = mTextureList.find(textureName);

	if (texture == mTextureList.end())
	{
		WriteLog(L"No texture in List", L_Warning);
		return;
	}

	D3D.GetContext()->VSSetShaderResources(startSlot, 1, &texture->second);
	D3D.GetContext()->HSSetShaderResources(startSlot, 1, &texture->second);
	D3D.GetContext()->DSSetShaderResources(startSlot, 1, &texture->second);
	D3D.GetContext()->PSSetShaderResources(startSlot, 1, &texture->second);
}

void RenderManager::SetRasterizerState(RS_Type type)
{
	mRasterizerState->SetState(type);
}

void RenderManager::SetTextureArray(wstring arrayName, UINT startSlot)
{
	auto textureArray = mTextureArrayList.find(arrayName);

	if (textureArray == mTextureArrayList.end())
	{
		WriteLog(L"No texture array in List", L_Warning);
		return;
	}

	D3D.GetContext()->VSSetShaderResources(startSlot, 1, &textureArray->second);
	D3D.GetContext()->HSSetShaderResources(startSlot, 1, &textureArray->second);
	D3D.GetContext()->DSSetShaderResources(startSlot, 1, &textureArray->second);
	D3D.GetContext()->PSSetShaderResources(startSlot, 1, &textureArray->second);
}

void RenderManager::SetBlendState(BS_Type type)
{
	mBlendState->SetState(type);
}

void RenderManager::SetDepthStencilState(DS_Type type)
{
	mDepthStencilState->SetState(type);
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