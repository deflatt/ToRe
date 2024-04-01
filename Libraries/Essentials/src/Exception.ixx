export module TR.Essentials.Exception;

import <string>;

export namespace TR {

	namespace Essentials {

		struct _Exception {
			const std::string msg;
			_Exception(std::string msg) : msg(msg) {}
		};

	}
	using namespace Essentials;

}