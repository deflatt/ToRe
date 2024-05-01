module TR.Graphics.Debugging;

namespace TR {

	namespace Graphics {

		namespace Debugging {

			void EnableDebugger()
			{
				HRESULT ret = D3D12GetDebugInterface(__uuidof(ID3D12Debug), &debugController);
				if (ret != 0) {
					throw FailedDebuggerCreationException(ret);
				}
				debugController->EnableDebugLayer();
			}

		}

	}

}