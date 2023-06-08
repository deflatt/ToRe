export module TR.Windows.Window;

export import <string>;
export import TR.Essentials.Procedure;
export import TR.Windows.Input;
export import TR.Essentials.Array;
export import TR.Windows.Exception;

#undef CreateWindow

export namespace TR::Windows {

	namespace Window {

		struct E_FailedClassRegistration : public _WinAPIException {
			E_FailedClassRegistration() : _WinAPIException("Failed to register windows class.") {}
		};
		struct E_FailedWindowCreation : public _WinAPIException {
			E_FailedWindowCreation() : _WinAPIException("Failed to create window.") {}
		};
		struct E_FailedWindowClosure : public _WinAPIException {
			E_FailedWindowClosure() : _WinAPIException("Failed to close window.") {}
		};

		struct _Context {
			HWND hwnd = nullptr;
			std::wstring wClassName = {};
			ProcedureSet<_WinEvent*> listeners = {};
			Procedure<_WinEvent*> msgListener = {};
		};

		void CreateClass(std::string className, UINT classStyles, WNDPROC proc = Input::WindowProc);
		void SetClass(_Context* window, std::string className);
		void CreateClass(_Context* window, std::string className, UINT classStyles, WNDPROC proc = Input::WindowProc);
		void CreateWindow(_Context* window, std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size); // Add support for parent, menu, lpParam
		
		void HandleMessages(_Context* window);

		void CloseWindow(_Context* window);

		void Release(_Context* window);

		// TODO: add quality of life functions like Show(), Hide(), Resize()

	}

	export struct _Window {

		Window::_Context window = {};

		void SetClass(std::string className);
		void CreateClass(std::string className, UINT classStyles, WNDPROC proc = Input::WindowProc);
		void CreateWindow(std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size); // Add support for parent, menu, lpParam

		void HandleMessages();

		void CloseWindow();

		~_Window();

	};

}