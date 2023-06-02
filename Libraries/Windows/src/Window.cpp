module;

#include <atlbase.h>
#include <atlconv.h>

module TR.Windows.Window;

#undef CreateWindow

namespace TR::Windows {

	namespace Context {

		void SetClass(_Context* context, std::string className)
		{
			context->wClassName = std::wstring(CA2W(className.c_str()));
		}

		void CreateClass(_Context* context, std::string className, UINT classStyles, WNDPROC proc)
		{
			context->wClassName = std::wstring(CA2W(className.c_str()));

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
			wndClass.lpszClassName = context->wClassName.c_str();
			wndClass.hIconSm = nullptr;

			RegisterClassEx(&wndClass); // Throws
		}

		void CreateWindow(_Context* context, std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size)
		{
			std::wstring wTitle(CA2W(title.c_str()));

			context->hwnd = CreateWindowEx(exStyles, context->wClassName.c_str(), wTitle.c_str(), styles, position[0]
				, position[1], size[0], size[1], nullptr, nullptr, nullptr, nullptr);

			if (!context->hwnd) {
				// Throws
			}

			context->msgListener = [context](_WinEvent* e) {
				if (e->msg.hwnd == context->hwnd)
					context->listeners(e);
			};

			Input::listeners.Insert(&context->msgListener);
		}

	}

}