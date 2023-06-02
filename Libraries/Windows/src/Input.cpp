module;

#include <Windows.h>

module TR.Windows.Input;

namespace TR::Windows {

	namespace Input {

		LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

	}

}