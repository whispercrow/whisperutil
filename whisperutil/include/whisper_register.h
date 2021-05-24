#ifndef WHISPER_REGISTER_H
#define WHISPER_REGISTER_H

#include "whisper_inc.h"
#include "whisper_macro.h"
#include "whisper_errorn.h"


namespace whisper
{
	using namespace std;

	bool Reg_AddPath(tstring szRegPath);
	bool Reg_DelPath(tstring szRegPath);
	
	bool Reg_SetValueString(tstring szRegPath, tstring szKey, tstring szValue);
	bool Reg_SetValueDWORD(tstring szRegPath, tstring szKey, DWORD nValue);
	bool Reg_SetValueDWORD64(tstring szRegPath, tstring szKey, ULONGLONG nValue);
	bool Reg_SetValueBinary(tstring szRegPath, tstring szKey, byte* pValue, size_t nBufferSize);

	size_t Reg_GetValueSize(tstring szRegPath, tstring szKey);
	size_t Reg_GetValue(tstring szRegPath, tstring szKey, byte* pData, size_t nBufferSize);

	tstring Reg_GetValueString(tstring szRegPath, tstring szKey);
	DWORD Reg_GetValueDWORD(tstring szRegPath, tstring szKey);
	ULONGLONG Reg_GetValueDWORD64(tstring szRegPath, tstring szKey);

	bool Reg_DelKey(tstring szRegPath, tstring szKey);


#ifdef WHISPER_REGISTER_INTERNAL
	HKEY Reg_GetBaseHKey(tstring szRegKeyPath);
	HKEY Reg_GetKeyHandle(tstring szRegKeyPath);
	tstring Reg_GetSubPath(tstring szRegKeyPath);	
#endif

} 



#endif // WHISPER_REGISTER_H