#pragma once
#include <winnt.h>
#include <locale>
#include <codecvt>
#include <comdef.h>
#include "log.h"


#define RETURN_ON_BAD_HR(expr) \
{ \
    HRESULT _hr_ = (expr); \
    if (FAILED(_hr_)) { \
	{\
		_com_error err(_hr_);\
		ERR(L"RETURN_ON_BAD_HR: %ls", err.ErrorMessage());\
	}\
		return _hr_; \
	} \
}

#define LOG_ON_BAD_HR(expr) \
{ \
    HRESULT _hr_ = (expr); \
    if (FAILED(_hr_)) { \
	{\
		_com_error err(_hr_);\
		ERR(L"BAD HR: %ls", err.ErrorMessage());\
	}\
	} \
}
class utilities
{
public:
	static std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	static std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

	// Create a string with last error message
	static std::wstring GetLastErrorStdWstr() {
		return s2ws(GetLastErrorStdStr());
	}
	// Create a string with last error message
	static std::string GetLastErrorStdStr()
	{
		DWORD error = GetLastError();
		if (error)
		{
			LPVOID lpMsgBuf;
			DWORD bufLen = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, nullptr);
			if (bufLen)
			{
				LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
				std::string result(lpMsgStr, lpMsgStr + bufLen);

				LocalFree(lpMsgBuf);

				return result;
			}
		}
		return std::string();
	}
};

