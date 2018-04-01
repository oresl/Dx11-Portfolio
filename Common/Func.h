#pragma once

namespace math
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

namespace dir
{
	static BOOL CheckDirectory(wstring path)
	{
		DWORD result = GetFileAttributes(path.c_str());

		// ��ΰ� �߸���
		if (result == INVALID_FILE_ATTRIBUTES)
			return FALSE;

		// ���丮
		if (result & FILE_ATTRIBUTE_DIRECTORY)
			return TRUE;

		// ���丮�� �ƴ�
		return FALSE;
	}
}