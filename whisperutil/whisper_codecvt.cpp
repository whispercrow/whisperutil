#include "include\whisper_codecvt.h"
using namespace std;

wstring whisper::ConvertUtf8ToUnicode(string szUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, szUtf8.c_str(), -1, NULL, 0);
	wchar_t* szUnicode = new wchar_t[(size_t)len + 1];
	memset(szUnicode, 0, ((size_t)len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, szUtf8.c_str(), -1, szUnicode, len);

	wstring szReturn = szUnicode;
	delete[] szUnicode;
	return szReturn;
}

string whisper::ConvertUnicodeToUtf8(wstring szUnicode)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, szUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	char* szUtf8 = new char[(size_t)len + 1];
	memset(szUtf8, 0, ((size_t)len + 1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, szUnicode.c_str(), -1, szUtf8, len, NULL, NULL);

	string szReturn = szUtf8;
	delete[] szUtf8;
	return szReturn;
}

wstring whisper::ConvertAnsiToUnicode(string szAnsi)
{
	int len = MultiByteToWideChar(CP_ACP, 0, szAnsi.c_str(), -1, NULL, 0);
	wchar_t* wszUnicode = new wchar_t[(size_t)len + 1];
	memset(wszUnicode, 0, ((size_t)len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, szAnsi.c_str(), -1, wszUnicode, len);

	wstring szReturn = wszUnicode;
	delete[] wszUnicode;
	return szReturn;
}

string whisper::ConvertUnicodeToAnsi(wstring szUnicode)
{
	int len = WideCharToMultiByte(CP_ACP, 0, szUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	char* szAnsi = new char[(size_t)len + 1];
	memset(szAnsi, 0, ((size_t)len + 1) * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, szUnicode.c_str(), -1, szAnsi, len, NULL, NULL);

	string szReturn = szAnsi;
	delete[] szAnsi;
	return szReturn;
}

string whisper::ConvertAnsiToUtf8(string szAnsi)
{
	int dwAnsiLen = MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), -1, NULL, NULL);
	wchar_t* szTmpUnicode = new wchar_t[(size_t)dwAnsiLen + 1];
	memset(szTmpUnicode, 0, sizeof(wchar_t) * ((size_t)dwAnsiLen + 1));
	MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), -1, szTmpUnicode, dwAnsiLen);

	int dwUtf8Len = WideCharToMultiByte(CP_UTF8, 0, szTmpUnicode, -1, NULL, 0, NULL, NULL);
	char* szUtf8 = new char[(size_t)dwUtf8Len + 1];
	memset(szUtf8, 0, sizeof(char) * ((size_t)dwUtf8Len + 1));
	WideCharToMultiByte(CP_UTF8, 0, szTmpUnicode, -1, szUtf8, dwUtf8Len, NULL, NULL);
	delete[] szTmpUnicode;

	string szReturn = szUtf8;
	delete[] szUtf8;
	return szReturn;
}

string whisper::ConvertUtf8ToAnsi(string szUtf8)
{
	int dwAnsiLen = MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), -1, NULL, NULL);
	wchar_t* szTmpUnicode = new wchar_t[(size_t)dwAnsiLen + 1];
	memset(szTmpUnicode, 0, sizeof(wchar_t) * ((size_t)dwAnsiLen + 1));
	MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), -1, szTmpUnicode, dwAnsiLen);

	int dwUtf8Len = WideCharToMultiByte(CP_ACP, 0, szTmpUnicode, -1, NULL, 0, NULL, NULL);
	char* szAnsi = new char[(size_t)dwUtf8Len + 1];
	memset(szAnsi, 0, sizeof(char) * ((size_t)dwUtf8Len + 1));
	WideCharToMultiByte(CP_ACP, 0, szTmpUnicode, -1, szAnsi, dwUtf8Len, NULL, NULL);
	delete[] szTmpUnicode;

	string szReturn = szAnsi;
	delete[] szAnsi;
	return szReturn;
}


