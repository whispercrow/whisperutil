#ifndef WHISPER_SERVICE_H
#define WHISPER_SERVICE_H

#include "whisper_inc.h"
#include "whisper_macro.h"
#include "whisper_errorn.h"

namespace whisper 
{
	bool Service_IsInstall(const tchar* szServiceName);
	bool Service_Install(
		const tchar* szServiceName, 
		const tchar* szServicePath, 
		const tchar* szServiceNameDisplay = NULL,
		const tchar* szServiceDiscription = NULL
	);
	bool Service_Delete(const tchar* szServiceName);
	bool Service_Start(const tchar* szServiceName);
	bool Service_Stop(const tchar* szServiceName);
	

#if 0
	BOOL StopDependentServices(const char* szServiceName);
#endif
}




#endif // WHISPER_SERVICE_H
