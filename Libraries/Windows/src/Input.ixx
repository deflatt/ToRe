export module TR.Windows.Input;

import <Windows.h>;
import <unordered_set>;
export import TR.Essentials.Proc;
export import TR.Windows.Exception;

namespace TR {

	namespace Windows {

		export namespace Input {

			struct _WinEvent {
				MSG msg;
				_WinEvent(MSG msg) : msg(msg) {}
			};

			struct _MessageEvent {
				MSG msg;
				bool returnDefaultProc;
				LRESULT returnValue;
				_MessageEvent(MSG msg, bool returnDefaultProc = true, LRESULT returnValue = 0)
					: msg(msg), returnDefaultProc(returnDefaultProc), returnValue(returnValue) {}
			};

			struct Listener {
				UINT msg;
				Proc<_MessageEvent*> onMessage;
			};

			LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

			void AddListener(Listener* listener);
			void RemoveListener(Listener* listener);

		}
		using namespace Input;

		namespace Input {

			std::unordered_map<UINT, std::unordered_set<Listener*>> listeners = {};

		}

	}
	using namespace Windows;

}