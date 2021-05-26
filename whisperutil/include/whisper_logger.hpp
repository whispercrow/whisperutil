#ifndef WHISPER_LOGGER_H
#define WHISPER_LOGGER_H

#include "whisper_inc.h"
#include "whisper_macro.h"
#include "whisper_errorn.h"

#include "whisper_path.h"

#define LOG_INFO {\
	tstringstream ss; \
	ss << std::left << std::setw(5) << _T("INFO") << _T(" "); \
	ss << _T('[') << std::left << whisper::GetFileName(__TFILE__); \
	ss << _T(':') << std::left << std::setw(3) << __LINE__ << _T(' '); \
	ss << _T("P:") << std::right << std::setw(5) << GetCurrentProcessId() << _T(' '); \
	ss << _T("T:") << std::right << std::setw(5) << GetCurrentThreadId(); \
	ss << _T(']'); \
	whisper::WisLogFile::szlogheader = ss.str(); \
	} (*(whisper::WisLogFile::GetInstance()))


#define LOG_WARN {\
	tstringstream ss; \
	ss << std::left << std::setw(5) << _T("WARN") << _T(" "); \
	ss << _T('[') << std::left << whisper::GetFileName(__TFILE__); \
	ss << _T(':') << std::left << std::setw(3) << __LINE__ << _T(' '); \
	ss << _T("P:") << std::right << std::setw(5) << GetCurrentProcessId() << _T(' '); \
	ss << _T("T:") << std::right << std::setw(5) << GetCurrentThreadId(); \
	ss << _T(']'); \
	whisper::WisLogFile::szlogheader = ss.str(); \
	} (*(whisper::WisLogFile::GetInstance()))


#define LOG_ERROR {\
	tstringstream ss; \
	ss << std::left << std::setw(5) << _T("ERROR") << _T(" "); \
	ss << _T('[') << std::left << whisper::GetFileName(__TFILE__); \
	ss << _T(':') << std::left << std::setw(3) << __LINE__ << _T(' '); \
	ss << _T("P:") << std::right << std::setw(5) << GetCurrentProcessId() << _T(' '); \
	ss << _T("T:") << std::right << std::setw(5) << GetCurrentThreadId(); \
	ss << _T(']'); \
	whisper::WisLogFile::szlogheader = ss.str(); \
	} (*(whisper::WisLogFile::GetInstance()))


#define LOG_FATAL {\
	tstringstream ss; \
	ss << std::left << std::setw(5) << _T("FATAL") << _T(" "); \
	ss << _T('[') << std::left << whisper::GetFileName(__TFILE__); \
	ss << _T(':') << std::left << std::setw(3) << __LINE__ << _T(' '); \
	ss << _T("P:") << std::right << std::setw(5) << GetCurrentProcessId() << _T(' '); \
	ss << _T("T:") << std::right << std::setw(5) << GetCurrentThreadId(); \
	ss << _T(']'); \
	whisper::WisLogFile::szlogheader = ss.str(); \
	} (*(whisper::WisLogFile::GetInstance()))



namespace whisper
{
	using namespace std;

	constexpr long long maxsize_logfile = 10 * 1024 * 1024;

	template<class T>
	class WisLog
	{
		template<typename P>
		friend basic_ostream<T>& operator <<(WisLog<T>& object, P param)
		{
			object.pre_log();

			object.m_ostream << param;

			object.post_log();
			return object.m_ostream;
		}

	protected:
		virtual void pre_log()
		{

		}

		virtual void post_log()
		{

		}

		WisLog<T>(basic_ostream<T>& ostream);

		inline static WisLog<T>* pThisObject = nullptr;

	private:
		WisLog<T>& operator=(const WisLog<T>&) = delete;
		WisLog<T>(WisLog<T>&) = delete;

		basic_ostream<T>& m_ostream;

	public:
		void				operator()(const T* data, ...);
		static void			InitLogger(basic_ostream<T>& ostream);
		static WisLog<T>* GetInstance();
	};

	template<class T>
	inline WisLog<T>::WisLog(basic_ostream<T>& ostream)
		:m_ostream{ ostream }
	{
	}

	template<class T>
	inline void WisLog<T>::InitLogger(basic_ostream<T>& ostream)
	{
		if (pThisObject == NULL)
		{
			pThisObject = new WisLog<T>{ ostream };
		}
	}

	template<class T>
	inline WisLog<T>* WisLog<T>::GetInstance()
	{
		return pThisObject;
	}


	template<class T>
	inline void WisLog<T>::operator()(const T* data, ...)
	{
		pThisObject->pre_log();

		pThisObject->m_ostream << data;
		pThisObject->m_ostream << endl;
		if (pThisObject->m_ostream.tellp() >= maxsize_logfile)
		{
			pThisObject->m_ostream.seekp(0, ios::beg);
		}

		pThisObject->post_log();
	}



	template<>
	inline void WisLog<char>::operator()(const char* data, ...)
	{
		pThisObject->pre_log();

		va_list vlArgs = nullptr;
		va_start(vlArgs, data);
		size_t nLen = _vscprintf(data, vlArgs) + 1;
		char* szBuffer = new(std::nothrow) char[nLen];
		if (nullptr != szBuffer)
		{
			_vsnprintf_s(szBuffer, nLen, nLen - 1, data, vlArgs);
			pThisObject->m_ostream << szBuffer;
			pThisObject->m_ostream << endl;
			delete[] szBuffer;
		}
		va_end(vlArgs);

		if (pThisObject->m_ostream.tellp() >= maxsize_logfile)
		{
			pThisObject->m_ostream.seekp(0, ios::beg);
		}

		pThisObject->post_log();
	}

	template<>
	inline void WisLog<wchar_t>::operator()(const wchar_t* data, ...)
	{
		pThisObject->pre_log();

		va_list vlArgs = nullptr;
		va_start(vlArgs, data);
		size_t nLen = _vscwprintf(data, vlArgs) + 1;
		wchar_t* szBuffer = new(std::nothrow) wchar_t[nLen];
		if (nullptr != szBuffer)
		{
			_vsnwprintf_s(szBuffer, nLen, nLen - 1, data, vlArgs);
			pThisObject->m_ostream << szBuffer;
			pThisObject->m_ostream << endl;
			delete[] szBuffer;
		}
		va_end(vlArgs);

		if (pThisObject->m_ostream.tellp() >= maxsize_logfile)
		{
			pThisObject->m_ostream.seekp(0, ios::beg);
		}

		pThisObject->post_log();
	}



	class WisLogFile : public WisLog<tchar>
	{
	private:
		WisLogFile();
		WisLogFile& operator=(const WisLogFile&) = delete;
		WisLogFile(WisLogFile&) = delete;

		virtual ~WisLogFile();

		virtual inline void pre_log();
		virtual inline void post_log();

		inline static basic_ofstream<tchar> m_filestream;

	public:
		inline static tstring szlogheader;
		static void	InitLogger(tstring szfilename);
	};

	inline whisper::WisLogFile::WisLogFile() :
		whisper::WisLog<tchar>(m_filestream)
	{

	}

	inline WisLogFile::~WisLogFile()
	{
		if (true == m_filestream.is_open())
		{
			m_filestream.close();
		}
	}


	inline void whisper::WisLogFile::InitLogger(tstring szfilename)
	{
		if (true == m_filestream.is_open())
		{
			m_filestream.close();
		}

		m_filestream.open(szfilename, ios::out | ios::app);
		pThisObject = new WisLogFile;
	}

	inline void whisper::WisLogFile::pre_log()
	{
		SYSTEMTIME sys{0};
		GetLocalTime(&sys);

		tstringstream ss;
		ss << setw(4) << setfill(_T('0')) << sys.wYear << _T('-');
		ss << setw(2) << setfill(_T('0')) << sys.wMonth << _T('-');
		ss << setw(2) << setfill(_T('0')) << sys.wDay << _T(' ');
		ss << setw(2) << setfill(_T('0')) << sys.wHour << _T(':');
		ss << setw(2) << setfill(_T('0')) << sys.wMinute << _T(':');
		ss << setw(2) << setfill(_T('0')) << sys.wSecond << _T(':');
		ss << setw(3) << setfill(_T('0')) << sys.wMilliseconds << _T(' ');
		m_filestream << ss.str();

		m_filestream << szlogheader;
	}

	inline void whisper::WisLogFile::post_log()
	{
		if (m_filestream.tellp() >= maxsize_logfile)
		{
			m_filestream.seekp(0, ios::beg);
		}

		szlogheader.clear();
	}
}

#endif // WHISPER_LOGGER_H