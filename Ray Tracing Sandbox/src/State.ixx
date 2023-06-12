module;

#include <Windows.h>

export module State;

export import TR.Windows.KeyboardListener;

using namespace TR;

export namespace State {

	bool active = false;

	void UpdateMouse(HWND hwnd) {
		if (active) {
			RECT rect;
			GetWindowRect(hwnd, &rect);
			ClipCursor(&rect);
			while (ShowCursor(FALSE) >= 0) {}
		}
		else {
			ClipCursor(nullptr);
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
			while (ShowCursor(TRUE) < 0) {}
		}
	}

	void Init(HWND hwnd) {
		Procedure<Windows::Input::_KeydownEvent*> toggleStateProc = [hwnd](Windows::Input::_KeydownEvent* e) {
			if (e->key == VK_ESCAPE) {
				active = !active;
				State::UpdateMouse(hwnd);
			}
		};
		Windows::Input::listeners.Add(toggleStateProc);
		UpdateMouse(hwnd);
	}

}