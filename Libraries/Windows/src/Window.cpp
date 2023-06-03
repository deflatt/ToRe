module;

#include <atlbase.h>
#include <atlconv.h>

module TR.Windows.Window;

#undef CreateWindow

namespace TR::Windows {

	namespace Context {

		void TR::Windows::Context::CreateClass(std::string className, UINT classStyles, WNDPROC proc)
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

	void _Context::SetClass(std::string className)
	{
		Context::SetClass(&context, className);
	}

	void _Context::CreateClass(std::string className, UINT classStyles, WNDPROC proc)
	{
		Context::CreateClass(&context, className, classStyles, proc);
	}

	void _Context::CreateWindow(std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size)
	{
		Context::CreateWindow(&context, title, styles, exStyles, position, size);
	}

	void _Context::HandleMessages()
	{
		Context::HandleMessages(&context);
	}

	const Context::_Context* _Context::GetContext() const noexcept
	{
		return &context;
	}

	Context::_Context* _Context::GetContext() noexcept
	{
		return &context;
	}

	void _Context::CloseWindow()
	{
		Context::CloseWindow(&context);
	}

	_Context::~_Context()
	{
		Context::Release(&context);
	}

}