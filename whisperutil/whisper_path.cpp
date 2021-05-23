#include "include/whisper_path.h"


std::tstring whisper::GetFileDir(tstring szFile)
{
	tstring::size_type nPosition = szFile.rfind(_T('\\'));
	nPosition = nPosition == -1 ? szFile.rfind(_T('/')) : nPosition;
	return szFile.substr(0, nPosition + 1);
}

std::tstring whisper::GetFileName(tstring szFile)
{
	tstring::size_type nPosition = szFile.rfind(_T('\\'));
	nPosition = nPosition == -1 ? szFile.rfind(_T('/')) : nPosition;
	return szFile.substr(nPosition + 1);
}

std::tstring whisper::GetFileSuffix(tstring szFile)
{
	tstring szFileName = GetFileName(szFile);
	tstring::size_type nPosition = szFileName.rfind(_T('.'));
	return nPosition == -1 ? _T("") : szFileName.substr(nPosition + 1);
}



std::tstring whisper::GetThisModulePath(HMODULE hModule)
{
	TCHAR szThisModulePath[WHISPER_MAX_PATH]{ 0 };
	DWORD dwLen = GetModuleFileName(hModule, szThisModulePath, _countof(szThisModulePath));
	if (0 == dwLen || _countof(szThisModulePath) == dwLen || _T('\0') == szThisModulePath[0])
	{
		return _T("");
	}

	return szThisModulePath;
}

std::tstring whisper::GetThisModuleDir(HMODULE hModule)
{
	tstring szModulePath = GetThisModulePath(hModule);
	tstring::size_type nPosition = szModulePath.rfind(_T('\\'));
	if (-1 == nPosition)
	{
		nPosition = szModulePath.rfind(_T('/'));
	}

	return nPosition == -1 ? _T("") : szModulePath.substr(0, nPosition + 1);
}

std::tstring whisper::GetThisModuleName(HMODULE hModule)
{
	tstring szModulePath = GetThisModulePath(hModule);
	return GetFileName(szModulePath);;
}

std::tstring whisper::GenThisDirFilePath(tstring szFileName, HMODULE hModule)
{
	tstring szModuleDir = GetThisModuleDir(hModule);
	return szModuleDir + szFileName;
}











