#include <iostream>
#include <Windows.h>
#undef CreateWindow

import TR.Essentials.Array;
import TR.Essentials.Procedure;
import TR.Windows.Window;
using namespace TR;

int main() {

	Windows::Context::_Context window = {};
	Windows::Context::CreateClass(&window, "ToRe Window Class", 0);
	Windows::Context::CreateWindow(&window, "ToRe Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, { 100, 100 }, { 1280, 720 });

	while (true);

	return 0;
}