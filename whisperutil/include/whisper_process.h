#ifndef WHISPER_PROCESS_H
#define WHISPER_PROCESS_H

#include "whisper_inc.h"
#include "whisper_errorn.h"
#include "whisper_macro.h"


namespace whisper
{
	using namespace std;

	enum class CALLBACK_RET { CALLBACK_CONTINUE, CALLBACK_BREAK };
	bool EnumSnapProcess(std::function<CALLBACK_RET(const PPROCESSENTRY32, void*)> pfunCallBackProcess, void* pUser);

	DWORD IsProcessRun(tstring szProcessName);
	DWORD IsFullPathProcessRun(tstring szProcessFullPath);

	tstring GetProcessFullPathFromID(DWORD nProcessID);

	bool TerminateProcessByID(DWORD nProcessID);

	bool SmartProcessIdToSessionId(DWORD dwProcessId, DWORD* pSessionId);
	DWORD CreateTokenProcess(tstring szProcessPath, DWORD dwTokenProcessID, DWORD dwSessionID);
	
}



#endif //WHISPER_PROCESS_H