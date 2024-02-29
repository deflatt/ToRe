module;

#include "NetworkingHeader.h"

export module TR.Networking.IOSocket;

export import TR.Networking.Socket;

export namespace TR {

	namespace Networking {

		struct E_ConnectionClosed : public _Exception {
			E_ConnectionClosed() : _Exception("Connection was closed while trying to receive data.") {}
		};
		struct E_FailedReceival : public _WinsockException {
			E_FailedReceival() : _WinsockException("Failed to receive data from socket.") {}
		};
		struct E_FailedSend : public _WinsockException {
			E_FailedSend() : _WinsockException("Failed to send data from socket.") {}
		};
		struct E_IncompleteSend : public _Exception {
			int lengthSent;
			E_IncompleteSend(int lengthSent) : _Exception("Failed to send data from socket."), lengthSent(lengthSent) {}
		};

		struct _ReceiveEvent : public _SocketEvent { using _SocketEvent::_SocketEvent; };
		struct _SendEvent : public _SocketEvent { using _SocketEvent::_SocketEvent; };

		namespace Socket {

			void UpdateIO(_Socket* sock, WSANETWORKEVENTS& netEvents, Procedure<_SocketEvent*> proc);

			int ReceiveFrom(_Socket* sock, char* data, int maxLength);
			void SendTo(_Socket* sock, char* data, int length);

		}
		using namespace Socket;

	}
	using namespace Networking;

}