module;

#include "NetworkingHeader.h"

export module TR.Networking.Exception;

export import TR.Essentials.Exception;

export namespace TR {

	namespace Networking {

		struct _WinsockException : public _Exception {
			HRESULT returnValue;
			int wsErrorCode;
			_WinsockException(std::string msg = {}) : _Exception(msg), wsErrorCode(GetLastError()) {}
		};

	}
	using namespace Networking;

}