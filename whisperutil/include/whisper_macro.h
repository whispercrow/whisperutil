#ifndef WHISPER_MACRO_H
#define WHISPER_MACRO_H


#ifdef _UNICODE

#define tchar				wchar_t
#define tstring				wstring
#define tcout				wcout
#define tstringstream		wstringstream
#define tostringstream		wostringstream
#define tistringstream		wistringstream

#define __TFILE__			__FILEW__
#define __TFUNCTION__		__FUNCTIONW__

#else

#define tchar				char
#define tstring				string
#define tcout				cout
#define tstringstream		stringstream
#define tostringstream		ostringstream
#define tistringstream		istringstream

#define __TFILE__			__FILE__
#define __TFUNCTION__		__FUNCTION__

#endif // _UNICODE






#endif // WHISPER_MACRO_H
