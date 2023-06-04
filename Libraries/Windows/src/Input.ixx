module;

#include <Windows.h>

export module TR.Windows.Input;
import TR.Essentials.Procedure;
import TR.Essentials.Event;

namespace TR::Windows {

	export struct _WinEvent : public _Event {
		MSG msg;
		_WinEvent(MSG msg = {}) : msg(msg) {}
	};

	export struct _WinMessageEvent : public _WinEvent {
		bool returnDefaultProc;
		LRESULT returnValue;
		_WinMessageEvent(MSG msg = {}, bool returnDefaultProc = true, LRESULT returnValue = 0) : _WinEvent(msg), returnDefaultProc(returnDefaultProc), returnValue(returnValue) {}
	};

	export namespace Input {

		ProcedureSet<_WinEvent*> listeners = {};

		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

		template <typename T_event>
		void HandleNewEvent(T_event event) {
			listeners(&event);
		}

	}

	export using Listener = Procedure<_WinMessageEvent*>;

}