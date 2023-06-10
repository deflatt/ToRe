module;

#include <Windows.h>

export module TR.Windows.RawMouseListener;

export import TR.Windows.MouseListener;
export import TR.Windows.Exception;

export namespace TR::Windows::Input {

	struct _RawMouseEvent : public _WinEvent {
		bool foreground;
		_RawMouseEvent(MSG msg, bool foreground) : _WinEvent(msg), foreground(foreground) {}
	};

	struct _RawMouseMoveEvent : public _RawMouseEvent {
		Long2 movement;
		_RawMouseMoveEvent(MSG msg, bool foreground, Long2 movement) : _RawMouseEvent(msg, foreground), movement(movement) {}
	};

	struct _RawMouseButtonEvent : public _RawMouseEvent {
		_MouseButton mouseButton;
		_RawMouseButtonEvent(MSG msg, bool foreground, _MouseButton mouseButton) : _RawMouseEvent(msg, foreground), mouseButton(mouseButton) {}
	};

	struct _RawMouseButtonDownEvent : public _RawMouseButtonEvent { using _RawMouseButtonEvent::_RawMouseButtonEvent; };
	struct _RawMouseButtonUpEvent : public _RawMouseButtonEvent { using _RawMouseButtonEvent::_RawMouseButtonEvent; };

	struct _RawMouseWheelEvent : public _RawMouseEvent {
		SHORT delta;
		_RawMouseWheelEvent(MSG msg, bool foreground, SHORT delta) : _RawMouseEvent(msg, foreground), delta(delta) {}
	};

	namespace RawMouseListener {

		struct E_FailedRawInputRegistration : public _WinAPIException {
			E_FailedRawInputRegistration() : _WinAPIException("Failed to register raw input.") {}
		};

		void Register(HWND hwnd);

	}

	extern Listener rawMouseListener;

}