#include <iostream>
#include <Windows.h>
#undef CreateWindow

import TR.Windows.Window;
import TR.Windows.KeyboardListener;
import TR.Essentials.Procedure;
using namespace TR;

int main() {

	try {
		Windows::Context::CreateClass("ToRe Window Class", 0);
		Windows::_Context window = {};
	
		Windows::Input::listeners.Insert(&Windows::Input::keyboardListener);
	
		window.SetClass("ToRe Window Class");
		window.CreateWindow("ToRe Sandbox", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, { 100, 100 }, { 1280, 720 });
	
		Procedure<Windows::Input::_KeydownEvent*> proc = [](Windows::Input::_KeydownEvent* e) {
			std::cout << "Pressed " << e->key << std::endl;
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