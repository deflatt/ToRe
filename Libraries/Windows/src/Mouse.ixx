export module TR.Windows.Mouse;

import <Windows.h>;
export import TR.Essentials.Definitions;
export import TR.Windows.Exception;
export import TR.Windows.Input;
export import TR.Essentials.Vec;

export namespace TR {

	namespace Windows {

		namespace Input {

			enum class _MouseButton { LEFT, RIGHT, MIDDLE, X1, X2 };

			struct _MouseEvent : public _WinEvent {
				Vec<short, 2> pos;
				_MouseEvent(MSG msg, Vec<short, 2> pos) : _WinEvent(msg), pos(pos) {}
			};
			struct _MouseMoveEvent : public _MouseEvent { using _MouseEvent::_MouseEvent; };

			Listener MouseMoveListener(Proc<_MouseMoveEvent> onMouseMove);

			struct _RawMouseEvent : public _WinEvent {
				bool foreground;
				_RawMouseEvent(MSG msg, bool foreground) : _WinEvent(msg), foreground(foreground) {}
			};
			struct _RawMouseMoveEvent : public _RawMouseEvent {
				Int2 movement;
				_RawMouseMoveEvent(MSG msg, bool foreground, Int2 movement) : _RawMouseEvent(msg, foreground), movement(movement) {}
			};
			struct _RawMouseButtonEvent : public _RawMouseEvent {
				_MouseButton button;
				_RawMouseButtonEvent(MSG msg, bool foreground, _MouseButton button) : _RawMouseEvent(msg, foreground), button(button) {}
			};
			struct _RawMouseButtonDownEvent : public _RawMouseButtonEvent { using _RawMouseButtonEvent::_RawMouseButtonEvent; };
			struct _RawMouseButtonUpEvent : public _RawMouseButtonEvent { using _RawMouseButtonEvent::_RawMouseButtonEvent; };

			struct FailedRawInputRegistrationException : public _WinAPIException {
				FailedRawInputRegistrationException() : _WinAPIException("Failed to register raw input.") {}
			};

			void RegisterRawMouseInput(HWND hwnd);

			Listener RawMouseListener(Proc<_RawMouseMoveEvent> onMouseMove, Proc<_RawMouseButtonDownEvent> onButtonDown, Proc<_RawMouseButtonUpEvent> onButtonUp);

		}
		using namespace Input;

	}
	using namespace Windows;

}