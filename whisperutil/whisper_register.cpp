#define WHISPER_REGISTER_INTERNAL
#include "include/whisper_register.h"
using namespace std;


HKEY whisper::Reg_GetKeyHandle(tstring szRegKeyPath)
{
	if (true == szRegKeyPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return NULL;
	}

	HKEY hRegBaseKey = Reg_GetBaseHKey(szRegKeyPath);
	if (NULL == hRegBaseKey)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return NULL;
	}

	tstring szSubPath = Reg_GetSubPath(szRegKeyPath);
	if (true == szSubPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return NULL;
	}

	HKEY hReceiveKey = NULL;
	LSTATUS nStatus = RegOpenKeyEx(hRegBaseKey, szSubPath.c_str(), NULL, KEY_ALL_ACCESS, &hReceiveKey);
	if (ERROR_SUCCESS != nStatus)
	{
		SetLastError(nStatus);
		return NULL;
	}

	return hReceiveKey;
}

HKEY whisper::Reg_GetBaseHKey(tstring szRegKeyPath)
{
	if (true == szRegKeyPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return NULL;
	}

	tstring::size_type nPosition = szRegKeyPath.find(_T('\\'));
	nPosition = nPosition != -1 ? nPosition : szRegKeyPath.find(_T('/'));
	if (-1 == nPosition)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return NULL;
	}

	HKEY hBaseKey = ERROR_SUCCESS;

	tstring sz_BaseKey = szRegKeyPath.substr(0, nPosition);
	if (sz_BaseKey == _T("HKEY_CLASSES_ROOT"))
	{
		hBaseKey = HKEY_CLASSES_ROOT;
	}
	else if (sz_BaseKey == _T("HKEY_CURRENT_CONFIG"))
	{
		hBaseKey = HKEY_CURRENT_CONFIG;
	}
	else if (sz_BaseKey == _T("HKEY_CURRENT_USER"))
	{
		hBaseKey = HKEY_CURRENT_USER;
	}
	else if (sz_BaseKey == _T("HKEY_USERS"))
	{
		hBaseKey = HKEY_USERS;
	}
	else if (sz_BaseKey == _T("HKEY_LOCAL_MACHINE"))
	{
		hBaseKey = HKEY_LOCAL_MACHINE;
	}
	else
	{
		SetLastError(WHISPER_ERR_PARAM);
		return NULL;
	}

	return hBaseKey;
}

tstring whisper::Reg_GetSubPath(tstring szRegKeyPath)
{
	if (true == szRegKeyPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return _T("");
	}

	tstring::size_type nPosition = szRegKeyPath.find(_T('\\'));
	nPosition = nPosition != -1 ? nPosition : szRegKeyPath.find(_T('/'));
	if (-1 == nPosition)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return _T("");
	}

	return szRegKeyPath.substr(nPosition + 1);
}


bool whisper::Reg_AddPath(tstring szRegPath)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	HKEY hRegBaseKey = Reg_GetBaseHKey(szRegPath);
	if (NULL == hRegBaseKey)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	tstring szSubPath = Reg_GetSubPath(szRegPath);
	if (true == szSubPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	HKEY hReceiveKey = NULL;
	LSTATUS nStatus = RegCreateKeyEx(hRegBaseKey, szSubPath.c_str(), NULL, NULL, NULL, KEY_CREATE_SUB_KEY, NULL, &hReceiveKey, NULL);
	if (ERROR_SUCCESS != nStatus)
	{
		SetLastError(nStatus);
		return false;
	}

	RegCloseKey(hReceiveKey);
	return true;
}


bool whisper::Reg_DelPath(tstring szRegPath)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	HKEY hRegBaseKey = Reg_GetBaseHKey(szRegPath);
	if (NULL == hRegBaseKey)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	tstring szSubPath = Reg_GetSubPath(szRegPath);
	if (true == szSubPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	LSTATUS nStatus = RegDeleteTree(hRegBaseKey, szSubPath.c_str());
	if (ERROR_SUCCESS != nStatus)
	{
		SetLastError(nStatus);
		return false;
	}

	return true;
}


bool whisper::Reg_SetValueString(tstring szRegPath, tstring szKey, tstring szValue)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return false;
	}

	LSTATUS nStatus = RegSetValueEx(hRegKey, szKey.c_str(), NULL, REG_SZ, (byte *)szValue.c_str(), sizeof(tchar) * (szValue.size() + 1));
	if (ERROR_SUCCESS != nStatus)
	{
		SetLastError(nStatus);
		RegCloseKey(hRegKey);
		return false;
	}

	RegCloseKey(hRegKey);
	return true;

}

bool whisper::Reg_SetValueDWORD(tstring szRegPath, tstring szKey, DWORD nValue)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return false;
	}

	LSTATUS nStatus = RegSetValueEx(hRegKey, szKey.c_str(), NULL, REG_DWORD, (byte*)&nValue, sizeof(nValue));
	if (ERROR_SUCCESS != nStatus)
	{
		SetLastError(nStatus);
		RegCloseKey(hRegKey);
		return false;
	}

	RegCloseKey(hRegKey);
	return true;
}

bool whisper::Reg_SetValueDWORD64(tstring szRegPath, tstring szKey, ULONGLONG nValue)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return false;
	}

	LSTATUS nStatus = RegSetValueEx(hRegKey, szKey.c_str(), NULL, REG_QWORD, (byte*)&nValue, sizeof(nValue));
	if (ERROR_SUCCESS != nStatus)
	{
		SetLastError(nStatus);
		RegCloseKey(hRegKey);
		return false;
	}

	RegCloseKey(hRegKey);
	return true;
}

bool whisper::Reg_SetValueBinary(tstring szRegPath, tstring szKey, byte* pValue, size_t nBufferSize)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return false;
	}

	LSTATUS nStatus = RegSetValueEx(hRegKey, szKey.c_str(), NULL, REG_BINARY, pValue, nBufferSize);
	if (ERROR_SUCCESS != nStatus)
	{
		SetLastError(nStatus);
		RegCloseKey(hRegKey);
		return false;
	}

	RegCloseKey(hRegKey);
	return true;
}

size_t whisper::Reg_GetValue(tstring szRegPath, tstring szKey, byte* pData, size_t nBufferSize)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return -1;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return -1;
	}

	LSTATUS nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, NULL, pData, (DWORD*)&nBufferSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return -1;
	}

	RegCloseKey(hRegKey);
	return nBufferSize;
}


size_t whisper::Reg_GetValueSize(tstring szRegPath, tstring szKey)
{
	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return -1;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return -1;
	}

	DWORD nNeedSize = 0;
	LSTATUS nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, NULL, NULL, &nNeedSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return -1;
	}

	RegCloseKey(hRegKey);
	return nNeedSize;
}

tstring whisper::Reg_GetValueString(tstring szRegPath, tstring szKey)
{
	DWORD nQueryType = REG_SZ;

	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return _T("");
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return _T("");
	}

	DWORD nNeedSize = 0;
	LSTATUS nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, &nQueryType, NULL, &nNeedSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return _T("");
	}

	byte* pValueData = new byte[nNeedSize]{0};

	nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, &nQueryType, pValueData, (DWORD*)&nNeedSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return _T("");
	}
	
	tstring szValue = (tchar *)pValueData;
	delete[] pValueData;

	RegCloseKey(hRegKey);
	SetLastError(WHISPER_ERR_SUCCESS);
	return szValue;
}

DWORD whisper::Reg_GetValueDWORD(tstring szRegPath, tstring szKey)
{
	DWORD nQueryType = REG_DWORD;

	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return 0;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return 0;
	}

	DWORD nNeedSize = 0;
	LSTATUS nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, &nQueryType, NULL, &nNeedSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return 0;
	}

	byte* pValueData = new byte[nNeedSize]{ 0 };

	nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, &nQueryType, pValueData, (DWORD*)&nNeedSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return 0;
	}

	DWORD nValue = *(DWORD*)pValueData;
	delete[] pValueData;

	RegCloseKey(hRegKey);
	SetLastError(WHISPER_ERR_SUCCESS);
	return nValue;
}

ULONGLONG whisper::Reg_GetValueDWORD64(tstring szRegPath, tstring szKey)
{
	DWORD nQueryType = REG_QWORD;

	if (true == szRegPath.empty())
	{
		SetLastError(WHISPER_ERR_PARAM);
		return 0;
	}

	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return 0;
	}

	DWORD nNeedSize = 0;
	LSTATUS nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, &nQueryType, NULL, &nNeedSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return 0;
	}

	byte* pValueData = new byte[nNeedSize]{ 0 };

	nStatus = RegQueryValueEx(hRegKey, szKey.c_str(), NULL, &nQueryType, pValueData, (DWORD*)&nNeedSize);
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return 0;
	}

	ULONGLONG nValue = *(DWORD*)pValueData;
	delete[] pValueData;

	RegCloseKey(hRegKey);
	SetLastError(WHISPER_ERR_SUCCESS);
	return nValue;
}

bool whisper::Reg_DelKey(tstring szRegPath, tstring szKey)
{
	HKEY hRegKey = Reg_GetKeyHandle(szRegPath);
	if (hRegKey == NULL)
	{
		return false;
	}

	LSTATUS nStatus = RegDeleteValue(hRegKey, szKey.c_str());
	if (ERROR_SUCCESS != nStatus)
	{
		RegCloseKey(hRegKey);
		SetLastError(nStatus);
		return false;
	}

	RegCloseKey(hRegKey);
	SetLastError(WHISPER_ERR_SUCCESS);
	return true;
}
