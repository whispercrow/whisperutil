#include <iostream>
#include "include/whisperutil.h"

using namespace std;

int main()
{
	bool error = false;
	DWORD dwError = ERROR_SUCCESS;

	tstring szPath = LR"(HKEY_CURRENT_USER\SOFTWARE\7-Zip)";
	tstring szKey = LR"(hello)";
	tstring szValue = LR"(°∞whisperutil.exe°±(Win32): “—º”‘ÿ°∞C:\Windows\SysWOW64\kernel.appcore.dll°±°£)";
	DWORD nValue = 32654;

	ULONGLONG nData = whisper::Reg_GetValueDWORD64(szPath, szKey);
	tcout << nData << endl;


	system("pause");
	return 0;
}
