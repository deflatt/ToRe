module TR.Graphics.CommandQueue;

import TR.Graphics.Device;

namespace TR {

	namespace Graphics {

		namespace CommandQueue {

			void Init(_CommandQueue* cmdQueue)
			{
				D3D12_COMMAND_QUEUE_DESC desc = {};
				HRESULT ret = device->CreateCommandQueue(&desc, __uuidof(ID3D12CommandQueue), &cmdQueue->cmdQueue);
				if (ret != 0) {
					throw _FailedCommandQueueCreationException(ret);
				}
			}

			void Execute(_CommandQueue* cmdQueue, ID3D12CommandList* const* lists, UINT numLists)
			{
				cmdQueue->cmdQueue->ExecuteCommandLists(numLists, lists);
			}

			void Execute(_CommandQueue* cmdQueue, ID3D12CommandList* list)
			{
				ID3D12CommandList* cmdLists[1] = { list };
				Execute(cmdQueue, cmdLists, 1);
			}

		}

	}

}