module;

#include <Windows.h>

export module TR.Windows.RawKeyboardListener;

export import TR.Windows.KeyboardListener;
export import TR.Windows.Exception;

export namespace TR::Windows::Input {

	struct _RawKeyboardEvent : public _WinEvent {
		bool foreground;
		USHORT key;
		_RawKeyboardEvent(MSG msg, bool foreground, USHORT key) : _WinEvent(msg), foreground(foreground), key(key) {}
	};

	struct _RawKeydownEvent : public _RawKeyboardEvent { using _RawKeyboardEvent::_RawKeyboardEvent; };
	struct _RawKeyupEvent : public _RawKeyboardEvent { using _RawKeyboardEvent::_RawKeyboardEvent; };

	namespace RawKeyboardListener {

		struct E_FailedRawInputRegistration : public _WinAPIException {
			E_FailedRawInputRegistration() : _WinAPIException("Failed to register raw input.") {}
		};

		void Register(HWND hwnd);

	}

	extern Listener rawKeyboardListener;

}