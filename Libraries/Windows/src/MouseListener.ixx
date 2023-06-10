module;

#include <Windows.h>

export module TR.Windows.MouseListener;

export import TR.Essentials.Array;
export import TR.Windows.Input;

export namespace TR::Windows::Input {

	enum class _MouseButton { LEFT, RIGHT, MIDDLE, X1, X2 };
	enum class _MouseKey {
		CONTROL = MK_CONTROL,
		LEFT_MOUSE = MK_LBUTTON,
		MIDDLE_MOUSE = MK_MBUTTON,
		RIGHT_MOUSE = MK_RBUTTON,
		SHIFT = MK_SHIFT,
		X1_MOUSE = MK_XBUTTON1,
		X2_MOUSE = MK_XBUTTON2
	};

	struct _MouseEvent : public _WinEvent {
		Int2 position;
		_MouseEvent(MSG msg, Int2 position) : _WinEvent(msg), position(position) {}
	};

	struct _MouseMoveEvent : public _MouseEvent { using _MouseEvent::_MouseEvent; };
	
	struct _MouseButtonEvent : public _MouseEvent {
		_MouseButton mouseButton;
		_MouseButtonEvent(MSG msg, _MouseButton mouseButton, Int2 position) : _MouseEvent(msg, position), mouseButton(mouseButton) {}
	};

	struct _MouseButtonDownEvent : public _MouseButtonEvent { using _MouseButtonEvent::_MouseButtonEvent; };
	struct _MouseButtonUpEvent : public _MouseButtonEvent { using _MouseButtonEvent::_MouseButtonEvent; };

	struct _MouseWheelEvent : public _MouseEvent {
		SHORT delta;
		_MouseWheelEvent(MSG msg, Int2 position, SHORT delta) : _MouseEvent(msg, position), delta(delta) {}
	};

	extern Listener mouseListener;

}