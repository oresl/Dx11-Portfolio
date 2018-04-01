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