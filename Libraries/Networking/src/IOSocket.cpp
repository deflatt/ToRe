module;

#include "NetworkingHeader.h"

module TR.Networking.IOSocket;

namespace TR::Networking {

	namespace Socket {

		void UpdateIO(_Socket* sock, WSANETWORKEVENTS& netEvents, Procedure<_SocketEvent*> proc)
		{
			if (netEvents.lNetworkEvents & FD_READ) {
				auto event = _ReceiveEvent(sock, netEvents.iErrorCode[FD_READ_BIT]);
				proc(&event);
			}
			if (netEvents.lNetworkEvents & FD_WRITE) {
				auto event = _SendEvent(sock, netEvents.iErrorCode[FD_WRITE_BIT]);
				proc(&event);
			}
		}

		int ReceiveFrom(_Socket* sock, char* data, int maxLength)
		{
			int length = recv(sock->sock, data, maxLength, 0);
			if (length < 0) {
				throw E_FailedReceival();
			}
			else if (length == 0) {
				throw E_ConnectionClosed();
			}
			return length;
		}

		void SendTo(_Socket* sock, char* data, int length)
		{
			int lengthSent = send(sock->sock, data, length, 0);
			if (lengthSent < 0) {
				throw E_FailedSend();
			}
			else if (lengthSent < length) {
				throw E_IncompleteSend(lengthSent);
			}
		}

	}

}