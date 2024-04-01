module TR.Windows.Input;

import <Windows.h>;

namespace TR {

	namespace Windows {

		namespace Input {

			LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
			{
				_MessageEvent msgEvent({ hwnd, msg, wParam, lParam });

				if (listeners.count(msg)) {
					for (Listener* listener : listeners.at(msg))
						listener->onMessage(&msgEvent);
				}

				if (msgEvent.returnDefaultProc)
					return DefWindowProc(hwnd, msg, wParam, lParam);
				else
					return msgEvent.returnValue;
			}

			void AddListener(Listener* listener)
			{
				listeners[listener->msg].insert(listener);
			}

			void RemoveListener(Listener* listener)
			{
				listeners[listener->msg].erase(listener);
			}

		}

	}

}