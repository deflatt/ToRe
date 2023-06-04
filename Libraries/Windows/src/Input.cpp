module TR.Windows.Input;

import <Windows.h>;

namespace TR::Windows {

	namespace Input {

		LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			_WinMessageEvent msgEvent({ hwnd, msg, wParam, lParam });
			
			listeners(&msgEvent);

			if (msgEvent.returnDefaultProc)
				return DefWindowProc(hwnd, msg, wParam, lParam);
			else
				return msgEvent.returnValue;
		}

	}

}