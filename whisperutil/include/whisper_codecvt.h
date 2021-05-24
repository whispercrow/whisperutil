#ifndef WHISPER_CODECVT_H
#define WHISPER_CODECVT_H

#include "whisper_inc.h"
#include "whisper_errorn.h"
#include "whisper_macro.h"


namespace whisper
{
	using namespace std;

	wstring ConvertUtf8ToUnicode(string szUtf8);
	string ConvertUnicodeToUtf8(wstring szUnicode);
	wstring ConvertAnsiToUnicode(string szAnsi);
	string ConvertUnicodeToAnsi(wstring szUnicode);
	string ConvertAnsiToUtf8(string szAnsi);
	string ConvertUtf8ToAnsi(string szUtf8);

}


#endif //WHISPER_CODECVT_H