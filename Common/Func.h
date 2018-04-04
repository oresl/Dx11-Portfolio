#pragma once

namespace func_math
{
	// 범위제한
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

		// 경로가 잘못됨
		if (result == INVALID_FILE_ATTRIBUTES)
			return false;

		// 디렉토리
		if (result & FILE_ATTRIBUTE_DIRECTORY)
			return true;

		// 디렉토리는 아님
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

		// 공용버퍼 (Pcb.fx)
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
		// 미리설정한 이미지로드시 desc
		DXGI_FORMAT format;
		if (gCFTexture.CompressedFormat)
		{
			/*
			DXGI_FORMAT_BC1_UNORM	세가지 색상, 1비트 알파
			DXGI_FORMAT_BC2_UNORM	세가지 색상, 4비트 알파
			DXGI_FORMAT_BC3_UNORM	세가지 색상, 8비트 알파
			DXGI_FORMAT_BC4_UNORM	하나의 색상 (그레이스케일)
			DXGI_FORMAT_BC5_UNORM	두가지 색상
			DXGI_FORMAT_BC6H_UF16	압축된 HDR이미지 자료
			DXGI_FORMAT_BC7_UNORM	고품질 RGBA압축 (노멀맵의 압축에 의한 오차가 줄어듬)
			*/

			if (grayscaled)
				format = DXGI_FORMAT_BC4_UNORM;
			else
				format = DXGI_FORMAT_BC3_UNORM;
		}
		else
			format = DXGI_FORMAT_FROM_FILE;

		// 기본설정으로 File원본로드 (추가적으로 format의 설정값을 통해 압축형식을 지정할 수 있으며, 이는 gpu메모리 요구량을 줄일 수 있다.)
		loadInfo->Width					= D3DX11_FROM_FILE;
		loadInfo->Height				= D3DX11_FROM_FILE;
		loadInfo->Depth					= D3DX11_FROM_FILE;
		loadInfo->FirstMipLevel			= 0;
		loadInfo->MipLevels				= D3DX11_FROM_FILE;
		loadInfo->Format				= format;
		loadInfo->Filter				= D3DX11_FILTER_NONE;
		loadInfo->MipFilter				= D3DX11_FILTER_LINEAR;
		loadInfo->pSrcInfo				= 0;

		// 배열로 설정시, 추가 업데이트를 위해 다음 플래그에 대해 추가설정값을 적용
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

		// 실제로 설정한 format이 적용됬는지 확인
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

		// Texture2D 생성
		vector<ID3D11Texture2D*> source;
		source.resize(size);
		HRESULT hr;

		for(UINT i = 0; i < size; ++i)
		{
			hr = D3DX11CreateTextureFromFile(device, textureNames[i].c_str(), &loadInfo, NULL, (ID3D11Resource**)&source[i], NULL);
			assert(SUCCEEDED(hr));
		}

		// 이미지의 desc
		D3D11_TEXTURE2D_DESC sourceDesc;
		source[0]->GetDesc(&sourceDesc);

		// 이미지배열의 desc
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

		// 배열생성
		ID3D11Texture2D* textureArray = NULL;
		hr = device->CreateTexture2D(&destDesc, 0, &textureArray);
		assert(SUCCEEDED(hr));

		// ** 해상도가 다른 파일끼리는 배열로 묶이지 않음
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

		// SRV desc설정
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