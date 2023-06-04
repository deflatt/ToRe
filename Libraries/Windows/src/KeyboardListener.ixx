export module TR.Windows.KeyboardListener;

export import TR.Essentials.Array;
export import TR.Windows.Input;

namespace TR::Windows::Input {

	export struct _KeyboardEvent : public _WinEvent {
		uchar key;
		ushort repeat;
		bool prev;
		_KeyboardEvent(MSG msg, uchar key, ushort repeat, bool prev) : _WinEvent(msg), key(key), repeat(repeat), prev(prev) {}
	};
	export struct _KeydownEvent : public _KeyboardEvent { using _KeyboardEvent::_KeyboardEvent; };
	export struct _KeyupEvent : public _KeyboardEvent { using _KeyboardEvent::_KeyboardEvent; };

	export extern Listener keyboardListener;

}