#include "include/whisper_service.h"

bool whisper::Service_IsInstall(const tchar* szServiceName)
{
	if (nullptr == szServiceName)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	bool bReturn = false;
	BOOL bError = FALSE;
	DWORD dwLastError = WHISPER_ERR_SUCCESS;

	SC_HANDLE hScm = NULL;
	SC_HANDLE hService = NULL;

	__try
	{
		hScm = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, GENERIC_READ);
		if (NULL == hScm)
		{
			dwLastError = GetLastError();
			__leave;
		}

		hService = OpenService(hScm, szServiceName, SERVICE_QUERY_CONFIG);
		if (NULL == hService)
		{
			dwLastError = GetLastError();
			__leave;
		}

		bReturn = true;
	}
	__finally
	{
		if (NULL != hScm)
		{
			CloseServiceHandle(hScm); hScm = NULL;
		}

		if (NULL != hService)
		{
			CloseServiceHandle(hService); hService = NULL;
		}
	}

	SetLastError(dwLastError);
	return bReturn;
}

bool whisper::Service_Install(
	const tchar* szServiceName, 
	const tchar* szServicePath, 
	const tchar* szServiceNameDisplay, 
	const tchar* szServiceDescription)
{
	if (nullptr == szServiceName || nullptr == szServicePath)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	bool bReturn = false;
	BOOL bError = FALSE;
	DWORD dwLastError = WHISPER_ERR_SUCCESS;

	SC_HANDLE hScm = NULL;
	SC_HANDLE hService = NULL;
	SERVICE_DESCRIPTION ServiceDescription{ 0 };
	ServiceDescription.lpDescription = (LPTSTR)szServiceDescription;

	__try
	{
		hScm = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
		if (NULL == hScm)
		{
			dwLastError = GetLastError();
			__leave;
		}

		hService = CreateService(
			hScm,
			szServiceName,
			szServiceNameDisplay ? szServiceNameDisplay : szServiceName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			szServicePath,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);
		if (FALSE == hService)
		{
			dwLastError = GetLastError();
			__leave;
		}

		ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &ServiceDescription);

		bReturn = true;
	}
	__finally
	{
		if (NULL != hScm)
		{
			CloseServiceHandle(hScm); hScm = NULL;
		}

		if (NULL != hService)
		{
			CloseServiceHandle(hService); hService = NULL;
		}
	}


	return bReturn;
}

bool whisper::Service_Delete(const tchar* szServiceName)
{
	if (nullptr == szServiceName)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	bool bReturn = false;
	BOOL bError = FALSE;
	DWORD dwLastError = WHISPER_ERR_SUCCESS;

	SC_HANDLE hScm = NULL;
	SC_HANDLE hService = NULL;

	__try
	{
		hScm = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
		if (NULL == hScm)
		{
			dwLastError = GetLastError();
			__leave;
		}

		hService = OpenService(hScm, szServiceName, DELETE);
		if (NULL == hService)
		{
			dwLastError = GetLastError();
			__leave;
		}

		bError = DeleteService(hService);
		if (FALSE == bError)
		{
			dwLastError = GetLastError();
			__leave;
		}

		bReturn = true;
	}
	__finally
	{
		if (NULL != hScm)
		{
			CloseServiceHandle(hScm); hScm = NULL;
		}

		if (NULL != hService)
		{
			CloseServiceHandle(hService); hService = NULL;
		}
	}


	return bReturn;
}

bool whisper::Service_Start(const tchar* szServiceName)
{
	if (nullptr == szServiceName)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	bool bReturn = false;
	BOOL bError = FALSE;
	DWORD dwLastError = WHISPER_ERR_SUCCESS;

	SC_HANDLE hScm = NULL;
	SC_HANDLE hService = NULL;
	DWORD dwBytesNeeded = 0;
	SERVICE_STATUS_PROCESS ServiceStatus{ 0 };
	ULONGLONG dwStartTickCount = GetTickCount64();
	DWORD dwOldCheckPoint = 0;

	__try
	{
		hScm = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
		if (NULL == hScm)
		{
			dwLastError = GetLastError();
			__leave;
		}

		hService = OpenService(hScm, szServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hService)
		{
			dwLastError = GetLastError();
			__leave;
		}

		bError = QueryServiceStatusEx(
			hService,
			SC_STATUS_PROCESS_INFO,
			(byte*)&ServiceStatus,
			sizeof(ServiceStatus),
			&dwBytesNeeded);
		if (FALSE == bError)
		{
			dwLastError = GetLastError();
			__leave;
		}

		if (ServiceStatus.dwCurrentState != SERVICE_STOPPED && ServiceStatus.dwCurrentState != SERVICE_STOP_PENDING)
		{
			bReturn = true;
			__leave;
		}

		dwOldCheckPoint = ServiceStatus.dwCheckPoint;

		while (ServiceStatus.dwCurrentState == SERVICE_STOP_PENDING)
		{
			DWORD dwWaitTime = ServiceStatus.dwWaitHint / 10;

			if (dwWaitTime < 1000)
			{
				dwWaitTime = 1000;
			}
			else if (dwWaitTime > 10000)
			{
				dwWaitTime = 10000;
			}

			Sleep(dwWaitTime);

			bError = QueryServiceStatusEx(
				hService,
				SC_STATUS_PROCESS_INFO,
				(byte*)&ServiceStatus,
				sizeof(ServiceStatus),
				&dwBytesNeeded);
			if (FALSE == bError)
			{
				dwLastError = WHISPER_ERR_TIMEOUT;
				__leave;
			}

			if (ServiceStatus.dwCheckPoint > dwOldCheckPoint)
			{
				dwStartTickCount = GetTickCount64();
				dwOldCheckPoint = ServiceStatus.dwCheckPoint;
			}
			else
			{
				if (GetTickCount64() - dwStartTickCount > ServiceStatus.dwWaitHint)
				{
					dwLastError = WHISPER_ERR_TIMEOUT;
					__leave;
				}
			}

		}

		bError = StartService(hService, 0, NULL);
		if (FALSE == bError)
		{
			dwLastError = GetLastError();
			__leave;
		}

		bError = QueryServiceStatusEx(
			hService,
			SC_STATUS_PROCESS_INFO,
			(byte*)&ServiceStatus,
			sizeof(ServiceStatus),
			&dwBytesNeeded);
		if (FALSE == bError)
		{
			dwLastError = GetLastError();
			__leave;
		}

		dwStartTickCount = GetTickCount64();
		dwOldCheckPoint = ServiceStatus.dwCheckPoint;

		while (ServiceStatus.dwCurrentState == SERVICE_START_PENDING)
		{
			DWORD dwWaitTime = ServiceStatus.dwWaitHint / 10;

			if (dwWaitTime < 1000)
			{
				dwWaitTime = 1000;
			}
			else if (dwWaitTime > 10000)
			{
				dwWaitTime = 10000;
			}

			Sleep(dwWaitTime);

			bError = QueryServiceStatusEx(
				hService,
				SC_STATUS_PROCESS_INFO,
				(byte*)&ServiceStatus,
				sizeof(ServiceStatus),
				&dwBytesNeeded);
			if (FALSE == bError)
			{
				dwLastError = GetLastError();
				__leave;
			}

			if (ServiceStatus.dwCheckPoint > dwOldCheckPoint)
			{
				dwStartTickCount = GetTickCount64();
				dwOldCheckPoint = ServiceStatus.dwCheckPoint;
			}
			else
			{
				if (GetTickCount64() - dwStartTickCount > ServiceStatus.dwWaitHint)
				{
					break;
				}
			}
		}

		if (ServiceStatus.dwCurrentState != SERVICE_RUNNING)
		{
			dwLastError = WHISPER_ERR_TIMEOUT;
			__leave;
		}

		bReturn = true;
	}
	__finally
	{
		if (NULL != hScm)
		{
			CloseServiceHandle(hScm); hScm = NULL;
		}

		if (NULL != hService)
		{
			CloseServiceHandle(hService); hService = NULL;
		}
	}


	return bReturn;
}

#if 0
bool whisper::StopDependentServices(tstring szServiceName)
{
	DWORD i;
	DWORD dwBytesNeeded;
	DWORD dwCount;

	LPENUM_SERVICE_STATUS   lpDependencies = NULL;
	ENUM_SERVICE_STATUS     ess;
	SC_HANDLE               schSCManager;
	SC_HANDLE               schService;
	SC_HANDLE               hDepService;
	SERVICE_STATUS_PROCESS  ssp;

	DWORD dwStartTime = GetTickCount();
	DWORD dwTimeout = 30000; // 30-second time-out

	schSCManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager)
	{
		return false;
	}

	schService = OpenService(
		schSCManager,         // SCM database 
		szServiceName,            // name of service 
		SERVICE_STOP |
		SERVICE_QUERY_STATUS |
		SERVICE_ENUMERATE_DEPENDENTS);
	if (NULL == schService)
	{
		CloseServiceHandle(schSCManager);
		return false;
	}

	// Pass a zero-length buffer to get the required buffer size.
	if (EnumDependentServices(schService, SERVICE_ACTIVE,
		lpDependencies, 0, &dwBytesNeeded, &dwCount))
	{
		// If the Enum call succeeds, then there are no dependent
		// services, so do nothing.
		CloseServiceHandle(schSCManager);
		CloseServiceHandle(schService);
		return true;
	}
	else
	{
		if (GetLastError() != ERROR_MORE_DATA)
		{
			CloseServiceHandle(schSCManager);
			CloseServiceHandle(schService);
			return false; // Unexpected error
		}


		// Allocate a buffer for the dependencies.
		lpDependencies = (LPENUM_SERVICE_STATUS)HeapAlloc(
			GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);

		if (!lpDependencies)
		{
			CloseServiceHandle(schSCManager);
			CloseServiceHandle(schService);
			return false;
		}

		__try {
			// Enumerate the dependencies.
			if (!EnumDependentServices(schService, SERVICE_ACTIVE,
				lpDependencies, dwBytesNeeded, &dwBytesNeeded,
				&dwCount))
			{
				CloseServiceHandle(hDepService);
				CloseServiceHandle(schSCManager);
				return false;
			}

			for (i = 0; i < dwCount; i++)
			{
				ess = *(lpDependencies + i);
				// Open the service.
				hDepService = OpenService(schSCManager,
					ess.lpServiceName,
					SERVICE_STOP | SERVICE_QUERY_STATUS);

				if (!hDepService)
				{
					CloseServiceHandle(hDepService);
					CloseServiceHandle(schSCManager);
					return false;
				}


				__try {
					// Send a stop code.
					if (!ControlService(hDepService,
						SERVICE_CONTROL_STOP,
						(LPSERVICE_STATUS)&ssp))
						return false;

					// Wait for the service to stop.
					while (ssp.dwCurrentState != SERVICE_STOPPED)
					{
						Sleep(ssp.dwWaitHint);
						if (!QueryServiceStatusEx(
							hDepService,
							SC_STATUS_PROCESS_INFO,
							(LPBYTE)&ssp,
							sizeof(SERVICE_STATUS_PROCESS),
							&dwBytesNeeded))
							return false;

						if (ssp.dwCurrentState == SERVICE_STOPPED)
							break;

						if (GetTickCount() - dwStartTime > dwTimeout)
							return false;
					}
				}
				__finally
				{
					// Always release the service handle.
					CloseServiceHandle(hDepService);
					CloseServiceHandle(schSCManager);
					CloseServiceHandle(schService);
				}
			}
		}
		__finally
		{
			// Always free the enumeration buffer.
			HeapFree(GetProcessHeap(), 0, lpDependencies);
		}
	}

	CloseServiceHandle(schSCManager);
	CloseServiceHandle(schService);
	return true;
}
#endif

bool whisper::Service_Stop(const tchar* szServiceName)
{
	if (nullptr == szServiceName)
	{
		SetLastError(WHISPER_ERR_PARAM);
		return false;
	}

	bool bReturn = false;
	BOOL bError = FALSE;
	DWORD dwLastError = WHISPER_ERR_SUCCESS;

	SC_HANDLE hScm = NULL;
	SC_HANDLE hService = NULL;
	DWORD dwBytesNeeded = 0;
	SERVICE_STATUS_PROCESS ServiceStatus{ 0 };
	ULONGLONG dwStartTime = GetTickCount64();
	DWORD dwTimeout = 30000;

	__try
	{
		hScm = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
		if (NULL == hScm)
		{
			dwLastError = GetLastError();
			__leave;
		}

		hService = OpenService(hScm, szServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS);
		if (NULL == hService)
		{
			dwLastError = GetLastError();
			__leave;
		}

		bError = QueryServiceStatusEx(
			hService,
			SC_STATUS_PROCESS_INFO,
			(byte*)&ServiceStatus,
			sizeof(ServiceStatus),
			&dwBytesNeeded);
		if (FALSE == bError)
		{
			dwLastError = GetLastError();
			__leave;
		}

		if (ServiceStatus.dwCurrentState == SERVICE_STOPPED)
		{
			bReturn = true;
			__leave;
		}

		while (ServiceStatus.dwCurrentState == SERVICE_STOP_PENDING)
		{
			DWORD dwWaitTime = ServiceStatus.dwWaitHint / 10;
			if (dwWaitTime < 1000)
			{
				dwWaitTime = 1000;
			}
			else if (dwWaitTime > 10000)
			{
				dwWaitTime = 10000;
			}

			Sleep(dwWaitTime);

			bError = QueryServiceStatusEx(
				hService,
				SC_STATUS_PROCESS_INFO,
				(byte*)&ServiceStatus,
				sizeof(ServiceStatus),
				&dwBytesNeeded);
			if (FALSE == bError)
			{
				dwLastError = GetLastError();
				__leave;
			}

			if (ServiceStatus.dwCurrentState == SERVICE_STOPPED)
			{
				bReturn = true;
				__leave;
			}

			if (GetTickCount64() - dwStartTime > dwTimeout)
			{
				dwLastError = WHISPER_ERR_TIMEOUT;
				__leave;
			}
		}

#if 0
		StopDependentServices(szServiceName);
#endif 

		bError = ControlService(hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ServiceStatus);
		if (FALSE == bError)
		{
			dwLastError = GetLastError();
			__leave;
		}

		while (ServiceStatus.dwCurrentState != SERVICE_STOPPED)
		{
			Sleep(ServiceStatus.dwWaitHint);

			bError = QueryServiceStatusEx(
				hService,
				SC_STATUS_PROCESS_INFO,
				(LPBYTE)&ServiceStatus,
				sizeof(SERVICE_STATUS_PROCESS),
				&dwBytesNeeded);
			if (FALSE == bError)
			{
				dwLastError = GetLastError();
				__leave;
			}

			if (ServiceStatus.dwCurrentState == SERVICE_STOPPED)
				break;

			if (GetTickCount64() - dwStartTime > dwTimeout)
			{
				dwLastError = WHISPER_ERR_TIMEOUT;
				__leave;
			}
		}

		bReturn = true;
		
	}
	__finally
	{
		if (NULL != hScm)
		{
			CloseServiceHandle(hScm); hScm = NULL;
		}

		if (NULL != hService)
		{
			CloseServiceHandle(hService); hService = NULL;
		}

	}

	return bReturn;
}
