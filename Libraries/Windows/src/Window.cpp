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

		void SetClass(_Context* context, std::string className)
		{
			context->wClassName = std::wstring(CA2W(className.c_str()));
		}

		void CreateClass(_Context* context, std::string className, UINT classStyles, WNDPROC proc)
		{
			CreateClass(className, classStyles, proc);
			SetClass(context, className);
		}

		void CreateWindow(_Context* context, std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size)
		{
			std::wstring wTitle(CA2W(title.c_str()));

			context->hwnd = CreateWindowEx(exStyles, context->wClassName.c_str(), wTitle.c_str(), styles, position[0]
				, position[1], size[0], size[1], nullptr, nullptr, nullptr, nullptr);

			if (!context->hwnd) {
				throw E_FailedWindowCreation();
			}

			context->msgListener = [context](_WinEvent* e) {
				if (e->msg.hwnd == context->hwnd)
					context->listeners(e);
			};

			Input::listeners.Insert(&context->msgListener);
		}

		void CloseWindow(_Context* context)
		{
			if (CloseWindow(context->hwnd) == 0) {
				throw E_FailedWindowClosure();
			}
		}

		void HandleMessages(_Context* context)
		{
			MSG msg = {};
			while (PeekMessage(&msg, context->hwnd, 0, 0, PM_REMOVE) > 0) {
				TranslateMessage(&msg); // Errors?
				DispatchMessage(&msg);
			}
		}

		void Release(_Context* context)
		{
			if (IsWindow(context->hwnd))
				DestroyWindow(context->hwnd);
			if (Input::listeners.Contains(&context->msgListener))
				Input::listeners.Remove(&context->msgListener);
		}

	}

	void _Window::SetClass(std::string className)
	{
		Window::SetClass(&context, className);
	}

	void _Window::CreateClass(std::string className, UINT classStyles, WNDPROC proc)
	{
		Window::CreateClass(&context, className, classStyles, proc);
	}

	void _Window::CreateWindow(std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size)
	{
		Window::CreateWindow(&context, title, styles, exStyles, position, size);
	}

	void _Window::HandleMessages()
	{
		Window::HandleMessages(&context);
	}

	void _Window::CloseWindow()
	{
		Window::CloseWindow(&context);
	}

	_Window::~_Window()
	{
		Window::Release(&context);
	}

}