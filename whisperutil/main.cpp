#include <iostream>
#include "include/whisperutil.h"

using namespace std;

int main()
{
	DWORD nSessionID = 0;
	bool bError = whisper::SmartProcessIdToSessionId(11548, &nSessionID);
	if (bError)
	{
		tcout << L"success: " << nSessionID << endl;
	}
	else
	{
		tcout << L"failed..." << endl;
	}

	system("pause");
	return 0;
}
