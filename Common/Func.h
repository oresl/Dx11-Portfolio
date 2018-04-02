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

namespace func_shader_buffer
{
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

	// Texture, SamplerState
}