module TR.Windows.KeyboardListener;

import <Windows.h>;

namespace TR::Windows::Input {

	Listener keyboardListener = [](_WinMessageEvent* event) {
		switch (event->msg.message) {
		case WM_KEYDOWN:
			Input::HandleNewEvent(_KeydownEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
			break;
		case WM_KEYUP:
			Input::HandleNewEvent(_KeyupEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
			break;
		default:
			break;
		}
	};

}