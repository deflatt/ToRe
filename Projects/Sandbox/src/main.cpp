#include <iostream>
#include <Windows.h>
#undef CreateWindow

import TR.Windows.Window;
using namespace TR;

int main() {

	try {
		Windows::Context::CreateClass("ToRe Window Class", 0);
		Windows::_Context window = {};

		window.SetClass("ToRe Window Class");
		window.CreateWindow("ToRe Sandbox", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, { 100, 100 }, { 1280, 720 });

		Procedure<Windows::_WinMessageEvent*> proc = [](Windows::_WinMessageEvent* e) {
			if (e->msg.message == WM_CHAR) {
				std::cout << "Typed " << (char)e->msg.wParam << std::endl;
			}
		};
		window.GetContext()->listeners.Insert(&proc);

		while (true) {
			window.HandleMessages();
		}

	}
	catch (_Exception e) {
		std::cout << e.msg << std::endl;
	}

	return 0;
}