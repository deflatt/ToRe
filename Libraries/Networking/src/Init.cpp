module;

#include "NetworkingHeader.h"

module TR.Networking.Init;

namespace TR::Networking {

	void InitWinsock()
	{
		WSADATA data = {};
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
			throw E_FailedWinsockInitialization();
		}
	}

}