#pragma once

namespace math
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

namespace dir
{
	static BOOL CheckDirectory(wstring path)
	{
		DWORD result = GetFileAttributes(path.c_str());

		// 경로가 잘못됨
		if (result == INVALID_FILE_ATTRIBUTES)
			return FALSE;

		// 디렉토리
		if (result & FILE_ATTRIBUTE_DIRECTORY)
			return TRUE;

		// 디렉토리는 아님
		return FALSE;
	}
}