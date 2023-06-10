module;

#include <Windows.h>
#include <windowsx.h>
#include <hidusage.h>

module TR.Windows.RawKeyboardListener;

namespace TR::Windows::Input {

	namespace RawKeyboardListener {

		void Register(HWND hwnd)
		{
			RAWINPUTDEVICE rid;
			rid.dwFlags = RIDEV_INPUTSINK;
			rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
			rid.usUsage = HID_USAGE_GENERIC_KEYBOARD;
			rid.hwndTarget = hwnd;

			BOOL ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
			if (!ret) {
				throw E_FailedRawInputRegistration();
			}
		}

	}

	Listener rawKeyboardListener = [](_WinMessageEvent* event) {
		if (event->msg.message != WM_INPUT)
			return;

		UINT inputSize = 0;
		GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* input = (RAWINPUT*)malloc(inputSize);
		GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, input, &inputSize, sizeof(RAWINPUTHEADER));

		if (input->header.dwType != RIM_TYPEKEYBOARD) {
			free(input);
			return;
		}
		bool foreground = !event->msg.wParam;

		if ((input->data.keyboard.Flags & 1) == 0)
			Input::HandleNewEvent(_RawKeydownEvent{ event->msg, foreground, input->data.keyboard.VKey });
		else if ((input->data.keyboard.Flags & 1) == 1)
			Input::HandleNewEvent(_RawKeyupEvent{ event->msg, foreground, input->data.keyboard.VKey });

		free(input);
	};

}