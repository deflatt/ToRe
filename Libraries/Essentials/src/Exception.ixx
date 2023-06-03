export module TR.Essentials.Exception;

export import <string>;

namespace TR {

	export struct _Exception {
		std::string msg;
		_Exception(std::string msg = {}) : msg(msg) {}
		virtual ~_Exception() {}
	};

}