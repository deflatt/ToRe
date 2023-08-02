module;

#include "NetworkingHeader.h"

export module TR.Networking.Init;

import TR.Networking.Exception;

export namespace TR {

	namespace Networking {

		struct E_FailedWinsockInitialization : public _WinsockException {
			E_FailedWinsockInitialization() : _WinsockException("Failed to initialize Winsock.") {}
		};

		void InitWinsock();

	}
	using namespace Networking;

}