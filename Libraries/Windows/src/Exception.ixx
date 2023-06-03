module;

#include <Windows.h>

export module TR.Windows.Exception;

export import TR.Essentials.Exception;

namespace TR::Windows {

	export struct _WinAPIException : public _Exception {
		DWORD winErrorCode;
		_WinAPIException(std::string msg = {}) : _Exception(msg), winErrorCode(GetLastError()) {}
	};

}