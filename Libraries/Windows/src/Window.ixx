export module TR.Windows.Window;

export import <Windows.h>;
export import <string>;
export import TR.Essentials.Procedure;
export import TR.Windows.Input;
export import TR.Windows.Input;
export import TR.Essentials.Array;
export import TR.Windows.Exception;

#undef CreateWindow

namespace TR::Windows {

	export namespace Context {

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
		void SetClass(_Context* context, std::string className);
		void CreateClass(_Context* context, std::string className, UINT classStyles, WNDPROC proc = Input::WindowProc);
		void CreateWindow(_Context* context, std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size); // Add support for parent, menu, lpParam
		
		void HandleMessages(_Context* context);

		void CloseWindow(_Context* context);

		void Release(_Context* context);

		// TODO: add quality of life functions like Show(), Hide(), Resize()

	}

	export struct _Context {

		void SetClass(std::string className);
		void CreateClass(std::string className, UINT classStyles, WNDPROC proc = Input::WindowProc);
		void CreateWindow(std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size); // Add support for parent, menu, lpParam

		void HandleMessages();

		void CloseWindow();

		_NODISCARD const Context::_Context* GetContext() const noexcept;
		_NODISCARD Context::_Context* GetContext() noexcept;

		~_Context();

		// TODO: add better getters / setters

	protected:

		Context::_Context context = {};

	};

}