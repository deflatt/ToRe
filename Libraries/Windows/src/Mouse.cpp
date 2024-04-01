module;

#include <Windows.h>
#include <windowsx.h>
#include <hidusage.h>

module TR.Windows.Mouse;

import TR.Windows.Input;

namespace TR {

	namespace Windows {

		namespace Input {

			Listener MouseMoveListener(Proc<_MouseMoveEvent> onMouseMove)
			{
				return { WM_MOUSEMOVE, [onMouseMove](_MessageEvent* event) {
					onMouseMove(_MouseMoveEvent{ event->msg, { GET_X_LPARAM(event->msg.lParam), GET_Y_LPARAM(event->msg.lParam) }});
				} };
			}

			void RegisterRawMouseInput(HWND hwnd)
			{
				RAWINPUTDEVICE rid;
				rid.dwFlags = RIDEV_INPUTSINK;
				rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
				rid.usUsage = HID_USAGE_GENERIC_MOUSE;
				rid.hwndTarget = hwnd;

				BOOL ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
				if (!ret) {
					throw FailedRawInputRegistrationException();
				}
			}

			Listener RawMouseListener(Proc<_RawMouseMoveEvent> onMouseMove, Proc<_RawMouseButtonDownEvent> onButtonDown, Proc<_RawMouseButtonUpEvent> onButtonUp)
			{
				return { WM_INPUT, [onMouseMove, onButtonDown, onButtonUp](_MessageEvent* event) {
					UINT inputSize = 0;
					GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, nullptr, &inputSize, sizeof(RAWINPUTHEADER));

					RAWINPUT* input = (RAWINPUT*)malloc(inputSize);
					GetRawInputData((HRAWINPUT)event->msg.lParam, RID_INPUT, input, &inputSize, sizeof(RAWINPUTHEADER));

					if (input->header.dwType != RIM_TYPEMOUSE) {
						free(input);
						return;
					}
					bool foreground = !event->msg.wParam;
					
					if (input->data.mouse.usFlags == MOUSE_MOVE_RELATIVE && onMouseMove)
						onMouseMove(_RawMouseMoveEvent{ event->msg, foreground, { input->data.mouse.lLastX, input->data.mouse.lLastY } });

					if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN && onButtonDown)
						onButtonDown(_RawMouseButtonDownEvent{ event->msg, foreground, _MouseButton::LEFT });
					else if (input->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP && onButtonUp)
						onButtonUp(_RawMouseButtonUpEvent{ event->msg, foreground, _MouseButton::LEFT });
					
					if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN && onButtonDown)
						onButtonDown(_RawMouseButtonDownEvent{ event->msg, foreground, _MouseButton::RIGHT });
					else if (input->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP && onButtonUp)
						onButtonUp(_RawMouseButtonUpEvent{ event->msg, foreground, _MouseButton::RIGHT });
					
					if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN && onButtonDown)
						onButtonDown(_RawMouseButtonDownEvent{ event->msg, foreground, _MouseButton::MIDDLE });
					else if (input->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP && onButtonUp)
						onButtonUp(_RawMouseButtonUpEvent{ event->msg, foreground, _MouseButton::MIDDLE });
					
					//if (input->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
					//	Input::HandleNewEvent(_RawMouseWheelEvent{ event->msg, foreground, (SHORT)input->data.mouse.usButtonData });

					free(input);

				} };
			}

		}
	}

}