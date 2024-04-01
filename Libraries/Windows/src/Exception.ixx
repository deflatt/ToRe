export module TR.Windows.Exception;

import <Windows.h>;
export import TR.Essentials.Exception;

export namespace TR {

	namespace Windows {

		struct _WinAPIException : public _Exception {
			DWORD winErrorCode;
			_WinAPIException(std::string msg) : _Exception(msg), winErrorCode(GetLastError()) {}
		};

	}
	using namespace Windows;

}