module;

#include "NetworkingHeader.h"

export module TR.Networking.Socket;

import TR.Networking.Exception;
import TR.Essentials.Event;
import TR.Essentials.Procedure;

export namespace TR {

	namespace Networking {

		struct E_FailedSocketInitialization : public _WinsockException {
			E_FailedSocketInitialization() : _WinsockException("Failed to initialize socket.") {}
		};
		struct E_FailedSocketClosure : public _WinsockException {
			E_FailedSocketClosure() : _WinsockException("Failed to close socket.") {}
		};
		struct E_FailedEventCreation() : public _WinsockException {
			E_FailedEventCreation() : _WinsockException("Failed to create socket event.") {}
		};
		struct E_FailedEventRegistration() : public _WinsockException {
			E_FailedEventRegistration() : _WinsockException("Failed to register socket events.") {}
		};

		struct _Socket {
			SOCKET sock = INVALID_SOCKET;
			HANDLE event = nullptr;
		};

		struct _SocketEvent : public _Event {
			_Socket* sock;
			int errorCode;
			_SocketEvent(_Socket* sock, int errorCode = 0) : sock(sock), errorCode(errorCode) {}
		};

		namespace Socket {

			void Init(_Socket* sock, int type = SOCK_STREAM, int family = AF_INET);
			void Close(_Socket* sock);

			int Receive(_Socket* sock, char* buf, int maxLength);
			int Send(_Socket* sock, const char* buf, int length);

			void RegisterEvents(_Socket* sock, LONG events);
			//void Update(_Socket* sock, Procedure<);

		}

	}
	using namespace Networking;

}