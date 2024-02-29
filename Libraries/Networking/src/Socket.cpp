module;

#include "NetworkingHeader.h"

module TR.Networking.Socket;

namespace TR::Networking {

	namespace Socket {

		void Init(_Socket* sock, int type, int family)
		{
			if (sock->sock != INVALID_SOCKET) {
				Close(sock);
			}
			sock->sock = socket(family, type, 0);
			if (sock->sock == INVALID_SOCKET) {
				throw E_FailedSocketInitialization();
			}
		}

		void Close(_Socket* sock)
		{
			if (closesocket(sock->sock) != 0) {
				sock->sock = INVALID_SOCKET;
				throw E_FailedSocketClosure();
			}
			sock->sock = INVALID_SOCKET;
		}

		int Receive(_Socket* sock, char* buf, int maxLength)
		{
			return recv(sock->sock, buf, maxLength, 0);
		}

		int Send(_Socket* sock, const char* buf, int length)
		{
			return send(sock->sock, buf, length, 0);
		}

		void RegisterEvents(_Socket* sock, LONG events)
		{
			if (sock->event) {
				WSACloseEvent(sock->event);
			}
			sock->event = WSACreateEvent();
			if (sock->event == WSA_INVALID_EVENT) {
				throw E_FailedEventCreation();
			}
			if (WSAEventSelect(sock->sock, sock->event, events) != 0) {
				throw E_FailedEventRegistration();
			}
		}

		void Update(_Socket* sock, Procedure<WSANETWORKEVENTS&> proc, DWORD wait)
		{
			DWORD ret = WaitForSingleObject(sock->event, wait);
			if (ret == WAIT_OBJECT_0) {
				WSANETWORKEVENTS netEvents;
				if (WSAEnumNetworkEvents(sock->sock, sock->event, &netEvents) != 0) {
					throw E_FailedEventInfoRetrieval();
				}
				proc(netEvents);
			}
			else if (ret == WAIT_FAILED) {
				throw E_FailedEventStatusRetrieval();
			}
		}

	}

}