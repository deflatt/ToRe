export module TR.Windows.Window;

import <Windows.h>;
#undef CreateWindow

export import <string>;
export import TR.Windows.Exception;
export import TR.Essentials.Vec;
export import TR.Windows.Input;

export namespace TR {

	namespace Windows {

		struct _Window {
			HWND hwnd = nullptr;
		};

		namespace Window {
			
			struct _FailedClassRegistrationException : public _WinAPIException {
				_FailedClassRegistrationException() : _WinAPIException("Failed to register window class.") {}
			};
			struct _FailedWindowCreationException : public _WinAPIException {
				_FailedWindowCreationException() : _WinAPIException("Failed to create window.") {}
			};
			struct _FailedWindowClosureException : public _WinAPIException {
				_FailedWindowClosureException() : _WinAPIException("Failed to close window.") {}
			};

			void CreateClass(std::string className, UINT classStyles = 0, WNDPROC proc = Input::WindowProc);
			void Create(_Window* window, std::string className, std::string title, Int2 size, Int2 position = { CW_USEDEFAULT, CW_USEDEFAULT }, DWORD styles = WS_OVERLAPPEDWINDOW, DWORD exStyles = 0);

			void HandleMessages(_Window* window);

			void Show(_Window* window);
			void Hide(_Window* window);
			
			void Close(_Window* window);

		}
		using namespace Window;

	}
	using namespace Windows;

}