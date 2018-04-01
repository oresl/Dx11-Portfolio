#pragma once
#include <ctime>

namespace writelog
{
	typedef enum TIMESTAMP_TYPE_ENUM
	{
		TS_DDMMYY = 0,
		TS_DDMMYY_HHMMSS,
		TS_YYMMDD,
		TS_YYMMDD_HHMMSS,
		TS_HHMMSS,
		TIMESTAMP_TYPE_COUNT,
	} TS_Type;

	static wstring GetTimeStamp(TS_Type type)
	{
		time_t rawTime;
		struct tm timeInfo;

		time(&rawTime);
		localtime_s(&timeInfo, &rawTime);

		WCHAR buff[64];
		WCHAR returnForm[64];

		// �ð���� : http://www.cplusplus.com/reference/ctime/strftime/
		switch (type)
		{
		case TS_DDMMYY:			wcscpy_s(returnForm, L"%d-%m-%Y"); break;
		case TS_DDMMYY_HHMMSS:	wcscpy_s(returnForm, L"%d-%m-%Y %H:%M:%S"); break;
		case TS_YYMMDD:			wcscpy_s(returnForm, L"%Y-%m-%d"); break;
		case TS_YYMMDD_HHMMSS:	wcscpy_s(returnForm, L"%Y-%m-%d %H:%M:%S"); break;
		case TS_HHMMSS:			wcscpy_s(returnForm, L"%H:%M:%S"); break;
		}

		wcsftime(buff, sizeof(buff), returnForm, &timeInfo);

		return data(buff);
	}

	typedef enum LOG_TYPE_ENUM
	{
		L_Error = 0,
		L_Warning,
		L_System,
		L_Information,
		L_Debug,
		LOG_TYPE_COUNT,
	} L_Type;

	static void WriteLog(wstring msg, L_Type type = L_Error)
	{
		// �α׷����� ���� ���
		if (!(gCFLog.Level >= (UINT)type))
			return;

		if (msg.size() == 0)
			return;

		// ���
		wstring path = L"log\\";

		if (FAILED(CheckDirectory(path)))
			CreateDirectory(path.c_str(), NULL);

		path += GetTimeStamp(TS_YYMMDD);
		path += L".log";

		// ���Ͼ��� (append���)
		wofstream file(path, wofstream::app);

		// �α�����
		wstring log = GetTimeStamp((TS_Type)gCFLog.TimeStampType);
		log += L"\t";

		switch (type)
		{
		case L_Debug:		log += L"Debug\t\t\t"; break;
		case L_Information:	log += L"Information\t\t"; break;
		case L_System:		log += L"System\t\t\t"; break;
		case L_Warning:		log += L"Warning\t\t\t"; break;
		case L_Error:		log += L"Error\t\t\t"; break;
		}

		log += msg;
		log += L"\n";

		file << log.c_str();
		file.close();
	}

	static void WriteShaderLog(wstring msg)
	{
		if (msg.size() == 0)
			return;

		// ���
		wstring path = L"log\\";

		if (FAILED(CheckDirectory(path)))
			CreateDirectory(path.c_str(), NULL);

		path += GetTimeStamp(TS_YYMMDD);
		path += L"-ShaderError.log";

		// ���Ͼ��� (append���)
		wofstream file(path, wofstream::app);

		// �α�����
		wstring log = GetTimeStamp((TS_Type)gCFLog.TimeStampType);
		log += L"\n";
		log += msg;
		log += L"\n";

		file << log.c_str();
		file.close();
	}
}