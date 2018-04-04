#pragma once

namespace func_math
{
	// ��������
	template <typename type>
	static type Clamp(type src, type min, type max)
	{
		if (min > max)
			swap(min, max);

		if (min > src)
			return min;

		if (max < src)
			return max;

		return src;
	}
}

namespace func_dir
{
	static bool CheckDirectory(wstring path)
	{
		DWORD result = GetFileAttributes(path.c_str());

		// ��ΰ� �߸���
		if (result == INVALID_FILE_ATTRIBUTES)
			return false;

		// ���丮
		if (result & FILE_ATTRIBUTE_DIRECTORY)
			return true;

		// ���丮�� �ƴ�
		return false;
	}
}

namespace func_shader
{
	// Buffer
	static void CreateBuffer(ID3D11Device* device, D3D11_BIND_FLAG flag, UINT byteWidth, D3D11_USAGE usage,
		void* data, ID3D11Buffer** buffer)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.BindFlags		= flag;
		bufferDesc.ByteWidth		= byteWidth;
		bufferDesc.Usage			= usage;

		if (usage == D3D11_USAGE_DYNAMIC)
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		else
			bufferDesc.CPUAccessFlags = 0;

		if (data != NULL)
		{
			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
			bufferData.pSysMem = data;

			HRESULT hr = device->CreateBuffer(&bufferDesc, &bufferData, buffer);
			assert(SUCCEEDED(hr));
		}
		else
		{
			HRESULT hr = device->CreateBuffer(&bufferDesc, NULL, buffer);
			assert(SUCCEEDED(hr));
		}
	}

	static void SetConstantBuffer(ID3D11DeviceContext* context, ID3D11Buffer* buffer, const void* data, UINT dataSize, UINT startSlot)
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		assert(SUCCEEDED(hr));

		memcpy(mapped.pData, data, (size_t)dataSize);

		// ������� (Pcb.fx)
		// 0 - World
		// 1 - Camera
		// 2 - Light

		context->Unmap(buffer, 0);
		context->VSSetConstantBuffers(startSlot, 1, &buffer);
		context->HSSetConstantBuffers(startSlot, 1, &buffer);
		context->DSSetConstantBuffers(startSlot, 1, &buffer);
		context->PSSetConstantBuffers(startSlot, 1, &buffer);
	}

	static void UpdateVertexBuffer(ID3D11DeviceContext* context, ID3D11Buffer* buffer, const void* data, UINT dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		assert(SUCCEEDED(hr));

		memcpy(mapped.pData, data, (size_t)dataSize);

		context->Unmap(buffer, 0);
	}

	// Texture
	static void LoadImageInfo(D3DX11_IMAGE_LOAD_INFO* loadInfo, bool grayscaled, bool textureArray)
	{
		// �̸������� �̹����ε�� desc
		DXGI_FORMAT format;
		if (gCFTexture.CompressedFormat)
		{
			/*
			DXGI_FORMAT_BC1_UNORM	������ ����, 1��Ʈ ����
			DXGI_FORMAT_BC2_UNORM	������ ����, 4��Ʈ ����
			DXGI_FORMAT_BC3_UNORM	������ ����, 8��Ʈ ����
			DXGI_FORMAT_BC4_UNORM	�ϳ��� ���� (�׷��̽�����)
			DXGI_FORMAT_BC5_UNORM	�ΰ��� ����
			DXGI_FORMAT_BC6H_UF16	����� HDR�̹��� �ڷ�
			DXGI_FORMAT_BC7_UNORM	��ǰ�� RGBA���� (��ָ��� ���࿡ ���� ������ �پ��)
			*/

			if (grayscaled)
				format = DXGI_FORMAT_BC4_UNORM;
			else
				format = DXGI_FORMAT_BC3_UNORM;
		}
		else
			format = DXGI_FORMAT_FROM_FILE;

		// �⺻�������� File�����ε� (�߰������� format�� �������� ���� ���������� ������ �� ������, �̴� gpu�޸� �䱸���� ���� �� �ִ�.)
		loadInfo->Width					= D3DX11_FROM_FILE;
		loadInfo->Height				= D3DX11_FROM_FILE;
		loadInfo->Depth					= D3DX11_FROM_FILE;
		loadInfo->FirstMipLevel			= 0;
		loadInfo->MipLevels				= D3DX11_FROM_FILE;
		loadInfo->Format				= format;
		loadInfo->Filter				= D3DX11_FILTER_NONE;
		loadInfo->MipFilter				= D3DX11_FILTER_LINEAR;
		loadInfo->pSrcInfo				= 0;

		// �迭�� ������, �߰� ������Ʈ�� ���� ���� �÷��׿� ���� �߰��������� ����
		if (textureArray)
		{
			loadInfo->Usage				= D3D11_USAGE_STAGING;
			loadInfo->BindFlags			= 0;
			loadInfo->CpuAccessFlags	= D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			loadInfo->MiscFlags			= 0;
		}
	}

	static void CreateShaderResourceView(ID3D11Device* device, wstring textureName, wstring extensionName, 
		ID3D11ShaderResourceView** srv, bool grayscaled = false)
	{
		wstring path = L"Data\\Texture\\" + textureName + L"." + extensionName;

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		LoadImageInfo(&loadInfo, grayscaled, false);

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(device, path.c_str(), &loadInfo, NULL, srv, NULL);
		assert(SUCCEEDED(hr));

		// ������ ������ format�� �������� Ȯ��
		ID3D11Texture2D* texture;
		(*srv)->GetResource((ID3D11Resource**)&texture);

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		texture->GetDesc(&textureDesc);
	}

	static void CreateShaderResourceViewArray(ID3D11Device* device, ID3D11DeviceContext* context, vector<wstring> textureNames, wstring extensionName,
		ID3D11ShaderResourceView** srvArray, bool grayscaled = false)
	{
		size_t size = textureNames.size();
		
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		LoadImageInfo(&loadInfo, grayscaled, true);

		// Texture2D ����
		vector<ID3D11Texture2D*> source;
		source.resize(size);
		HRESULT hr;

		for(UINT i = 0; i < size; ++i)
		{
			hr = D3DX11CreateTextureFromFile(device, textureNames[i].c_str(), &loadInfo, NULL, (ID3D11Resource**)&source[i], NULL);
			assert(SUCCEEDED(hr));
		}

		// �̹����� desc
		D3D11_TEXTURE2D_DESC sourceDesc;
		source[0]->GetDesc(&sourceDesc);

		// �̹����迭�� desc
		D3D11_TEXTURE2D_DESC destDesc;
		ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
		destDesc.Width              = sourceDesc.Width;
		destDesc.Height             = sourceDesc.Height;
		destDesc.MipLevels          = sourceDesc.MipLevels;
		destDesc.ArraySize          = (UINT)size;
		destDesc.Format             = sourceDesc.Format;
		destDesc.SampleDesc.Count   = 1;
		destDesc.SampleDesc.Quality = 0;
		destDesc.Usage              = D3D11_USAGE_DEFAULT;
		destDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		destDesc.CPUAccessFlags     = 0;
		destDesc.MiscFlags          = 0;

		// �迭����
		ID3D11Texture2D* textureArray = NULL;
		hr = device->CreateTexture2D(&destDesc, 0, &textureArray);
		assert(SUCCEEDED(hr));

		// ** �ػ󵵰� �ٸ� ���ϳ����� �迭�� ������ ����
		for(UINT element = 0; element < (UINT)size; element++)
		{
			for(UINT mipLevel = 0; mipLevel < sourceDesc.MipLevels; mipLevel++)
			{
				D3D11_MAPPED_SUBRESOURCE mapped;
				hr = context->Map(source[element], mipLevel, D3D11_MAP_READ, 0, &mapped);
				assert(SUCCEEDED(hr));

				context->UpdateSubresource
				(
					textureArray,
					D3D11CalcSubresource(mipLevel, element, sourceDesc.MipLevels),
					NULL,
					mapped.pData,
					mapped.RowPitch,
					mapped.DepthPitch
				);

				context->Unmap(source[element], mipLevel);
			}
		}	

		// SRV desc����
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format							= destDesc.Format;
		srvDesc.ViewDimension					= D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MostDetailedMip	= 0;
		srvDesc.Texture2DArray.MipLevels		= destDesc.MipLevels;
		srvDesc.Texture2DArray.FirstArraySlice	= 0;
		srvDesc.Texture2DArray.ArraySize		= (UINT)size;

		device->CreateShaderResourceView(textureArray, &srvDesc, srvArray);

		SafeReleaseCom(textureArray);

		for (UINT i = 0; i < size; ++i)
		{
			SafeReleaseCom(source[i]);
		}
	}
}