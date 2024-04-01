module TR.Graphics.CommandList;

import TR.Graphics.Device;

namespace TR {

	namespace Graphics {

		namespace CommandList {

			void Init(_CommandList* cmdList, ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators)
			{
				cmdList->numParallel = numParallel;
				cmdList->numAllocators = numAllocators;

				cmdList->allocators.resize(numAllocators);
				for (UINT i = 0; i < numAllocators; i++) {
					cmdList->allocators[i].resize(numParallel);
					for (UINT j = 0; j < numParallel; j++)
						Init(&cmdList->allocators[i][j]);
				}

				HRESULT ret = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT
					, cmdList->allocators[0][0].cmdAllocator.Get(), nullptr
					, __uuidof(ID3D12GraphicsCommandList), &cmdList->cmdList);
				if (ret != 0) {
					throw _FailedCommandListCreationException(ret);
				}
			}

			void Reset(_CommandList* cmdList, UINT allocator, UINT index)
			{
				Reset(&cmdList->allocators[allocator][index]);

				HRESULT ret = cmdList->cmdList->Reset(cmdList->allocators[allocator][index]
					.cmdAllocator.Get(), nullptr);
				if (ret != 0) {
					throw _FailedCommandListResetException(ret);
				}
			}

			void Close(_CommandList* cmdList)
			{
				HRESULT ret = cmdList->cmdList->Close();
				if (ret != 0) {
					throw _FailedCommandListClosureException(ret);
				}
			}

		}

	}

}