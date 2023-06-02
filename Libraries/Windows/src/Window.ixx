export module TR.Windows.Window;

export import <Windows.h>;
export import <string>;
export import TR.Essentials.Procedure;
export import TR.Windows.Input;
export import TR.Windows.Input;
export import TR.Essentials.Array;

#undef CreateWindow

namespace TR::Windows {

	export namespace Context {

		struct _Context {
			HWND hwnd = nullptr;
			std::wstring wClassName = {};
			ProcedureSet<_WinEvent*> listeners = {};
			Procedure<_WinEvent*> msgListener = {};
		};

		//void CreateClass(std::string className, UINT classStyles, WNDPROC proc = Input::WindowProc);
		void SetClass(_Context* context, std::string className);
		void CreateClass(_Context* context, std::string className, UINT classStyles, WNDPROC proc = Input::WindowProc);
		void CreateWindow(_Context* context, std::string title, DWORD styles, DWORD exStyles, Int2 position, Int2 size); // Add support for parent, menu, lpParam

	}

}