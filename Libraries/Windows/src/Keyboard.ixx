export module TR.Windows.Keyboard;

import <Windows.h>;
export import TR.Essentials.Definitions;
export import TR.Windows.Exception;
export import TR.Windows.Input;

export namespace TR {

	namespace Windows {

		namespace Input {

			struct _KeyEvent : public _WinEvent {
				uchar key;
				ushort repeatCount;
				bool prevState;
				_KeyEvent(MSG msg, uchar key, ushort repeatCount, bool prevState)
					: _WinEvent(msg), key(key), repeatCount(repeatCount), prevState(prevState) {}
			};
			struct _KeydownEvent : public _KeyEvent { using _KeyEvent::_KeyEvent; };
			struct _KeyupEvent : public _KeyEvent { using _KeyEvent::_KeyEvent; };
			struct _CharEvent : public _KeyEvent { using _KeyEvent::_KeyEvent; };
			
			Listener KeydownListener(Proc<_KeydownEvent> onKeydown);
			Listener KeyupListener(Proc<_KeyupEvent> onKeyup);
			Listener CharListener(Proc<_CharEvent> onChar);

		}
		using namespace Input;

	}
	using namespace Windows;

}