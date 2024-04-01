export module TR.Graphics.Debugging;

import <d3d12sdklayers.h>;
import TR.Graphics.Definitions;
import TR.Graphics.Exception;

export namespace TR {

	namespace Graphics {

		namespace Debugging {

			ComPtr<ID3D12Debug> debugController = {};

			struct _FailedDebuggerCreation : public _D3D12Exception {
				_FailedDebuggerCreation(HRESULT returnValue) : _D3D12Exception("Failed to create debugger.", returnValue) {}
			};

			void EnableDebugger();

		}
		using namespace Debugging;

	}
	using namespace Graphics;

}