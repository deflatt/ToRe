module;

#include <Windows.h>

export module TR.Windows.Input;
import TR.Essentials.Procedure;

namespace TR::Windows {

	export struct _WinEvent {
		MSG msg;
		_WinEvent(MSG msg = {}) : msg(msg) {}
		virtual ~_WinEvent() {}
	};

	export namespace Input {

		ProcedureSet<_WinEvent*> listeners = {};

		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

	}

}