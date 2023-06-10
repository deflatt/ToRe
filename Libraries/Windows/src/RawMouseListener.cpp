module;

#include <Windows.h>
#include <windowsx.h>
#include <hidusage.h>

module TR.Windows.RawMouseListener;

namespace TR::Windows::Input {

	namespace RawMouseListener {

		void Register(HWND hwnd)
		{
			RAWINPUTDEVICE rid;
			rid.dwFlags = RIDEV_INPUTSINK;
			rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
			rid.usUsage = HID_USAGE_GENERIC_MOUSE;
			rid.hwndTarget = hwnd;

			BOOL ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
			if (!ret) {
				throw E_FailedRawInputRegistration();
			}
		}

	}

	Listener rawMouseListener = [](_WinMessageEvent* event) {
		if (event->msg.message != WM_INPUT)
			return;

		UINT inputSize = 0;
		GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* input = (RAWINPUT*)malloc(inputSize);
		GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, input, &inputSize, sizeof(RAWINPUTHEADER));

		if (input->header.dwType != RIM_TYPEMOUSE) {
			free(input);
			return;
		}
		bool foreground = !event->msg.wParam;

		if (input->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
			Input::HandleNewEvent(_RawMouseMoveEvent{ event->msg, foreground, { input->data.mouse.lLastX, input->data.mouse.lLastY } });

		if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			Input::HandleNewEvent(_RawMouseButtonDownEvent{ event->msg, foreground, _MouseButton::LEFT });
		else if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
			Input::HandleNewEvent(_RawMouseButtonUpEvent{ event->msg, foreground, _MouseButton::LEFT });

		if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
			Input::HandleNewEvent(_RawMouseButtonDownEvent{ event->msg, foreground, _MouseButton::RIGHT });
		else if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
			Input::HandleNewEvent(_RawMouseButtonUpEvent{ event->msg, foreground, _MouseButton::RIGHT });

		if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
			Input::HandleNewEvent(_RawMouseButtonDownEvent{ event->msg, foreground, _MouseButton::MIDDLE });
		else if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
			Input::HandleNewEvent(_RawMouseButtonUpEvent{ event->msg, foreground, _MouseButton::MIDDLE });

		if (input->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
			Input::HandleNewEvent(_RawMouseWheelEvent{ event->msg, foreground, (SHORT)input->data.mouse.usButtonData });

		free(input);
	};

}