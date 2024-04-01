module TR.Windows.Keyboard;

import <Windows.h>;
import TR.Windows.Input;

namespace TR {

	namespace Windows {

		namespace Input {

			Listener KeydownListener(Proc<_KeydownEvent> onKeydown)
			{
				return { WM_KEYDOWN, [onKeydown](_MessageEvent* event) {
					onKeydown(_KeydownEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
				}};
			}

			Listener KeyupListener(Proc<_KeyupEvent> onKeyup)
			{
				return { WM_KEYUP, [onKeyup](_MessageEvent* event) {
					onKeyup(_KeyupEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
				}};
			}

			Listener CharListener(Proc<_CharEvent> onChar)
			{
				return { WM_CHAR, [onChar](_MessageEvent* event) {
					onChar(_CharEvent{ event->msg, (uchar)event->msg.wParam, LOWORD(event->msg.lParam), bool(event->msg.lParam & (1 << 30)) });
				}};
			}

		}

	}

}