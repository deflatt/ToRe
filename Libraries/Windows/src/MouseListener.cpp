module;

#include <Windows.h>
#include <windowsx.h>

module TR.Windows.MouseListener;

namespace TR::Windows::Input {

	Listener mouseListener = [](_WinMessageEvent* event) {
		Int2 pos = { GET_X_LPARAM(event->msg.lParam), GET_Y_LPARAM(event->msg.lParam) };
		if (event->msg.message == WM_MOUSEWHEEL)
			ScreenToClient(event->msg.hwnd, (LPPOINT)&pos);

		switch (event->msg.message) {
		case WM_MOUSEMOVE:
			Input::HandleNewEvent(_MouseMoveEvent{ event->msg, pos });
			break;
		case WM_LBUTTONDOWN:
			Input::HandleNewEvent(_MouseButtonDownEvent{ event->msg, _MouseButton::LEFT, pos });
			break;
		case WM_LBUTTONUP:
			Input::HandleNewEvent(_MouseButtonUpEvent{ event->msg, _MouseButton::LEFT, pos });
			break;
		case WM_RBUTTONDOWN:
			Input::HandleNewEvent(_MouseButtonDownEvent{ event->msg, _MouseButton::RIGHT, pos });
			break;
		case WM_RBUTTONUP:
			Input::HandleNewEvent(_MouseButtonUpEvent{ event->msg, _MouseButton::RIGHT, pos });
			break;
		case WM_MBUTTONDOWN:
			Input::HandleNewEvent(_MouseButtonDownEvent{ event->msg, _MouseButton::MIDDLE, pos });
			break;
		case WM_MBUTTONUP:
			Input::HandleNewEvent(_MouseButtonUpEvent{ event->msg, _MouseButton::MIDDLE, pos });
			break;
		case WM_XBUTTONDOWN:
			Input::HandleNewEvent(_MouseButtonDownEvent{ event->msg, HIWORD(event->msg.wParam) == XBUTTON1 ? _MouseButton::X1 : _MouseButton::X2, pos });
			break;
		case WM_XBUTTONUP:
			Input::HandleNewEvent(_MouseButtonUpEvent{ event->msg, HIWORD(event->msg.wParam) == XBUTTON1 ? _MouseButton::X1 : _MouseButton::X2, pos });
			break;
		case WM_MOUSEWHEEL:
			Input::HandleNewEvent(_MouseWheelEvent{ event->msg, pos, GET_WHEEL_DELTA_WPARAM(event->msg.wParam) });
			break;
		default:
			break;
		}
	};

}