#include <iostream>

namespace TR {

	namespace Graphics {

		namespace Context {

			struct _Context {

			};

			void Init(_Context* gfx){}

		}
		struct _Context {
			
			Context::_Context context = {};

			void Init() {
				Context::Init(&context);
			}

		};

	}

}

int main() {

	using namespace TR;
	using namespace TR::Graphics;

	_Context context = {};
	context.Init();

	return 0;
}