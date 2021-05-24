#include <userenv.h>
#include "include\whisper_process.h"
#include "include\whisper_path.h"

#pragma comment(lib, "userenv.lib")


using namespace std;

bool whisper::EnumSnapProcess(
	std::function<CALLBACK_RET(const PPROCESSENTRY32, void*)> pfunCallBackProcess,
	void* pUser)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap)
	{
		return false;
	}

	PROCESSENTRY32 stProcessEntry32 = { 0 };
	stProcessEntry32.dwSize = sizeof(stProcessEntry32);

	BOOL bError = Process32First(hProcessSnap, &stProcessEntry32);
	while (TRUE == bError)
	{
		CALLBACK_RET bBreak = pfunCallBackProcess(&stProcessEntry32, pUser);
		if (CALLBACK_RET::CALLBACK_BREAK == bBreak)
		{
			break;
		}

		bError = Process32Next(hProcessSnap, &stProcessEntry32);
	}


	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		CloseHandle(hProcessSnap);
		hProcessSnap = NULL;
	}

	SetLastError(WHISPER_ERR_SUCCESS);
	return true;
}


DWORD whisper::IsProcessRun(tstring szProcessName)
{
	if (true == szProcessName.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return 0;
	}

	transform(szProcessName.begin(), szProcessName.end(), szProcessName.begin(), ::toupper);

	DWORD nProcessID = 0;

	std::function pFunCheckExits
	{
		[szProcessName](const PPROCESSENTRY32 a, void* b)
		{
			tstring szExeUpper = a->szExeFile;
			transform(szExeUpper.begin(), szExeUpper.end(), szExeUpper.begin(), ::toupper);

			if (szProcessName == szExeUpper)
			{
				*(DWORD*)b = a->th32ProcessID;
				return CALLBACK_RET::CALLBACK_BREAK;
			}

			return CALLBACK_RET::CALLBACK_CONTINUE;
		},
	};

	bool bError = EnumSnapProcess(pFunCheckExits, &nProcessID);
	if (false == bError)
	{
		return 0;
	}

	return nProcessID;
}

DWORD whisper::IsFullPathProcessRun(tstring szProcessFullPath)
{
	if (true == szProcessFullPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return 0;
	}

	transform(szProcessFullPath.begin(), szProcessFullPath.end(), szProcessFullPath.begin(), ::toupper);

	DWORD nProcessID = 0;

	std::function pFunCheckExits
	{
		[szProcessFullPath](const PPROCESSENTRY32 a, void* b)
		{
			tstring szExeFullPathUpper = GetProcessFullPathFromID(a->th32ProcessID);
			transform(szExeFullPathUpper.begin(), szExeFullPathUpper.end(), szExeFullPathUpper.begin(), ::toupper);

			if (szProcessFullPath == szExeFullPathUpper)
			{
				*(DWORD*)b = a->th32ProcessID;
				return CALLBACK_RET::CALLBACK_BREAK;
			}

			return CALLBACK_RET::CALLBACK_CONTINUE;
		},
	};

	bool bError = EnumSnapProcess(pFunCheckExits, &nProcessID);
	if (false == bError)
	{
		return 0;
	}

	return nProcessID;
}

tstring whisper::GetProcessFullPathFromID(DWORD nProcessID)
{
	if (0 == nProcessID)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return _T("");
	}


	tstring szReturn{};

	DWORD nLastError = 0;
	HANDLE hProcess = NULL;
	DWORD dwLen = 0;
	tchar* szDiskArray = NULL;
	tchar* lpDiskDriver = NULL;
	tchar* szReplaceResult = NULL;
	tchar szDeviceFullPath[WHISPER_MAX_PATH] = { 0 };

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nProcessID);
	if (NULL == hProcess)
	{

		nLastError = GetLastError();
		goto FUN_CLEANUP;
	}


	dwLen = GetProcessImageFileName(hProcess, szDeviceFullPath, _countof(szDeviceFullPath));
	if (0 == dwLen)
	{
		nLastError = GetLastError();
		goto FUN_CLEANUP;
	}


	dwLen = GetLogicalDriveStrings(0, NULL);
	if (0 == dwLen)
	{
		nLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	szDiskArray = new tchar[dwLen];
	dwLen = GetLogicalDriveStrings(dwLen, szDiskArray);
	if (0 == dwLen)
	{
		nLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	lpDiskDriver = szDiskArray;
	while (_T('\0') != *lpDiskDriver)
	{
		tchar* szTagBegin = lpDiskDriver;

		size_t uLen = _tcslen(lpDiskDriver);
		lpDiskDriver += uLen + 1;


		if (_T('\\') == *(szTagBegin + uLen - 1))
		{
			*(szTagBegin + uLen - 1) = _T('\0');
		}

		tchar szTargetFormat[WHISPER_MAX_PATH] = { 0 };
		DWORD dwLen = QueryDosDevice(szTagBegin, szTargetFormat, _countof(szTargetFormat));
		if (0 == dwLen)
		{
			continue;
		}

		tchar* szResult = _tcsstr(szDeviceFullPath, szTargetFormat);
		if (NULL == szResult || szDeviceFullPath != szResult)
		{
			continue;
		}

		szReplaceResult = szDeviceFullPath + dwLen - 2 - _tcslen(szTagBegin);
		memcpy(szReplaceResult, szTagBegin, sizeof(tchar) * _tcslen(szTagBegin));
		break;
	}

	if (NULL == szReplaceResult)
	{
		nLastError = WHISPER_ERR_STRFORMAT;
		goto FUN_CLEANUP;
	}

	szReturn = szReplaceResult;

FUN_CLEANUP:
	if (NULL != hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}


	if (NULL != szDiskArray)
	{

		delete[] szDiskArray;
		szDiskArray = NULL;
	}


	SetLastError(nLastError);
	return szReturn;
}

bool whisper::TerminateProcessByID(DWORD nProcessID)
{
	if (0 == nProcessID)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return FALSE;
	}

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
	if (NULL == hProcess)
	{
		return FALSE;
	}

	BOOL bError = TerminateProcess(hProcess, 0);
	DWORD dwLastError = GetLastError();

	if (NULL != hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}

	SetLastError(dwLastError);
	return FALSE == bError ? false : true;
}

DWORD whisper::CreateTokenProcess(tstring szProcessPath, DWORD dwTokenProcessID, DWORD dwSessionID)
{
	if (true == szProcessPath.empty() || 0 == dwTokenProcessID)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return 0;
	}

	DWORD dwRetProcessID = 0;

	BOOL bError = FALSE;
	DWORD dwLastError = WHISPER_ERR_SUCCESS;
	HANDLE hDupProcess = NULL;
	HANDLE hDupToken = NULL;
	HANDLE hNewToken = NULL;
	LPVOID pEnv = NULL;
	DWORD dwTokenSessionID = dwSessionID;

	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = { 0 };

	tchar* szProcessCmd = new tchar[szProcessPath.size() + 1]{0};

	errno_t ErrorCode = _tcsncpy_s(szProcessCmd, szProcessPath.size() + 1, szProcessPath.c_str(), _TRUNCATE);
	if (0 != ErrorCode)
	{
		dwLastError = WHISPER_ERR_SMALLBUFFER;
		goto FUN_CLEANUP;
	}

	hDupProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwTokenProcessID);
	if (NULL == hDupProcess)
	{
		dwLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	bError = OpenProcessToken(hDupProcess, TOKEN_DUPLICATE, &hDupToken);
	if (FALSE == bError)
	{
		dwLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	bError = DuplicateTokenEx(hDupToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hNewToken);
	if (FALSE == bError)
	{
		dwLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	bError = SetTokenInformation(hNewToken, TokenSessionId, &dwTokenSessionID, sizeof(dwTokenSessionID));
	if (FALSE == bError)
	{
		dwLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	bError = CreateEnvironmentBlock(&pEnv, hNewToken, FALSE);
	if (FALSE == bError)
	{
		dwLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	bError = CreateProcessAsUser(hNewToken, NULL, szProcessCmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, pEnv, NULL, &si, &pi);
	if (FALSE == bError)
	{
		dwLastError = GetLastError();
		goto FUN_CLEANUP;
	}

	dwRetProcessID = pi.dwProcessId;

FUN_CLEANUP:
	if (NULL != pEnv)
	{
		DestroyEnvironmentBlock(pEnv);
		pEnv = NULL;
	}

	if (NULL != hNewToken)
	{
		CloseHandle(hNewToken);
		hNewToken = NULL;
	}

	if (NULL != hDupToken)
	{
		CloseHandle(hDupToken);
		hDupToken = NULL;
	}

	if (NULL != hDupProcess)
	{
		CloseHandle(hDupProcess);
		hDupProcess = NULL;
	}

	if (NULL != szProcessCmd)
	{
		delete[] szProcessCmd;
		szProcessCmd = NULL;
	}

	if (NULL != pi.hProcess)
	{
		CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
	}

	if (NULL != pi.hThread)
	{
		CloseHandle(pi.hThread);
		pi.hThread = NULL;
	}

	SetLastError(dwLastError);
	return dwRetProcessID;
}

bool whisper::SmartProcessIdToSessionId(DWORD dwProcessId, DWORD* pSessionId)
{
	typedef BOOL(WINAPI* LPFUNPROCESSIDTOSESSIONID)(_In_ DWORD dwProcessId, _Out_ DWORD* pSessionId);
	static LPFUNPROCESSIDTOSESSIONID FunProcessIdToSessionId = NULL;

	if (NULL == FunProcessIdToSessionId)
	{
		HMODULE hKernel32 = GetModuleHandle(_T("kernel32"));
		if (NULL == hKernel32)
		{
			SetLastError(WHISPER_ERR_MODULE);
			return false;
		}

		FunProcessIdToSessionId = (LPFUNPROCESSIDTOSESSIONID)GetProcAddress(hKernel32, "ProcessIdToSessionId");


		if (NULL == FunProcessIdToSessionId)
		{
			if (NULL != hKernel32)
			{
				CloseHandle(hKernel32);
				hKernel32 = NULL;
			}

			SetLastError(WHISPER_ERR_FUNCTION);
			return false;
		}
	}

	return FunProcessIdToSessionId(dwProcessId, pSessionId) ? true : false;
}
