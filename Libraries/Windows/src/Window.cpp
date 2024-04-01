module;

#include <atlbase.h>
#include <atlconv.h>
#undef CreateWindow

module TR.Windows.Window;

import TR.Windows.Exception;

namespace TR {

	namespace Windows {

		namespace Window {

			void CreateClass(std::string className, UINT classStyles, WNDPROC proc)
			{
				std::wstring wClassName = std::wstring(CA2W(className.c_str()));

				WNDCLASSEX wndClass = {};
				wndClass.cbSize = sizeof(WNDCLASSEX);
				wndClass.style = classStyles;
				wndClass.lpfnWndProc = proc;
				wndClass.cbClsExtra = 0;
				wndClass.cbWndExtra = 0;
				wndClass.hInstance = nullptr;
				wndClass.hIcon = nullptr;
				wndClass.hCursor = nullptr;
				wndClass.hbrBackground = nullptr;
				wndClass.lpszMenuName = nullptr;
				wndClass.lpszClassName = wClassName.c_str();
				wndClass.hIconSm = nullptr;

				if (RegisterClassEx(&wndClass) == 0) {
					throw _FailedClassRegistrationException();
				}
			}

			void Create(_Window* window, std::string className, std::string title, Int2 size, Int2 position, DWORD styles, DWORD exStyles)
			{
				std::wstring wClassName(CA2W(className.c_str()));
				std::wstring wTitle(CA2W(title.c_str()));

				window->hwnd = CreateWindowEx(exStyles, wClassName.c_str(), wTitle.c_str(), styles, position[0]
					, position[1], size[0], size[1], nullptr, nullptr, nullptr, nullptr);

				if (!window->hwnd) {
					throw _FailedWindowCreationException();
				}
			}

			void HandleMessages(_Window* window)
			{
				MSG msg = {};
				while (PeekMessage(&msg, window->hwnd, 0, 0, PM_REMOVE) > 0) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			void Show(_Window* window)
			{
				ShowWindow(window->hwnd, SW_SHOW);
			}

			void Hide(_Window* window)
			{
				ShowWindow(window->hwnd, SW_HIDE);
			}

			void Close(_Window* window)
			{
				if (CloseWindow(window->hwnd) == 0) {
					throw _FailedWindowClosureException();
				}
			}

		}

	}

}