module;

#include <atlbase.h>
#include <atlconv.h>
#undef CreateWindow

module TR.Windows.Window;

namespace TR::Windows {

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
				throw E_FailedClassRegistration();
			}
		}

		void SetClass(_Context* window, std::string className)
		{
			window->wClassName = std::wstring(CA2W(className.c_str()));
		}

		void CreateClass(_Context* window, std::string className, UINT classStyles, WNDPROC proc)
		{
			CreateClass(className, classStyles, proc);
			SetClass(window, className);
		}

		void CreateWindow(_Context* window, std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size)
		{
			std::wstring wTitle(CA2W(title.c_str()));

			window->hwnd = CreateWindowEx(exStyles, window->wClassName.c_str(), wTitle.c_str(), styles, position[0]
				, position[1], size[0], size[1], nullptr, nullptr, nullptr, nullptr);

			if (!window->hwnd) {
				throw E_FailedWindowCreation();
			}

			window->msgListener = [window](_WinEvent* e) {
				if (e->msg.hwnd == window->hwnd)
					window->listeners(e);
			};

			Input::listeners.Insert(&window->msgListener);
		}

		void CloseWindow(_Context* window)
		{
			if (CloseWindow(window->hwnd) == 0) {
				throw E_FailedWindowClosure();
			}
		}

		void HandleMessages(_Context* window)
		{
			MSG msg = {};
			while (PeekMessage(&msg, window->hwnd, 0, 0, PM_REMOVE) > 0) {
				TranslateMessage(&msg); // Errors?
				DispatchMessage(&msg);
			}
		}

		void Release(_Context* window)
		{
			if (IsWindow(window->hwnd))
				DestroyWindow(window->hwnd);
			if (Input::listeners.Contains(&window->msgListener))
				Input::listeners.Remove(&window->msgListener);
		}

	}

	void _Window::SetClass(std::string className)
	{
		Window::SetClass(&window, className);
	}

	void _Window::CreateClass(std::string className, UINT classStyles, WNDPROC proc)
	{
		Window::CreateClass(&window, className, classStyles, proc);
	}

	void _Window::CreateWindow(std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size)
	{
		Window::CreateWindow(&window, title, styles, exStyles, position, size);
	}

	void _Window::HandleMessages()
	{
		Window::HandleMessages(&window);
	}

	void _Window::CloseWindow()
	{
		Window::CloseWindow(&window);
	}

	_Window::~_Window()
	{
		Window::Release(&window);
	}

}