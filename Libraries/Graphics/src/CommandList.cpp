module;

#include "GraphicsHeader.h"

module TR.Graphics.CommandList;

namespace TR::Graphics {

	namespace CommandList {

		void Init(_Context* cmdList, ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators)
		{
			cmdList->numParallel = numParallel;
			cmdList->numAllocators = numAllocators;

			cmdList->allocators.resize(numAllocators);
			for (UINT i = 0; i < numAllocators; i++) {
				cmdList->allocators[i].resize(numParallel);
				for (UINT j = 0; j < numParallel; j++)
					cmdList->allocators[i][j].Init();
			}

			HRESULT ret = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT
				, cmdList->allocators[0][0].cmdAllocator.cmdAllocator.Get(), nullptr
				, __uuidof(ID3D12GraphicsCommandList), &cmdList->cmdList);
			if (ret != 0) {
				throw E_FailedCommandListCreation(ret);
			}
		}

		void Reset(_Context* cmdList, UINT allocator, UINT index)
		{
			cmdList->allocators[allocator][index].Reset();

			HRESULT ret = cmdList->cmdList->Reset(cmdList->allocators[allocator][index]
				.cmdAllocator.cmdAllocator.Get(), nullptr);
			if (ret != 0) {
				throw E_FailedCommandListReset(ret);
			}
		}

		void Close(_Context* cmdList)
		{
			HRESULT ret = cmdList->cmdList->Close();
			if (ret != 0) {
				throw E_FailedCommandListClosure(ret);
			}
		}

	}

	void _CommandList::Init(ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators)
	{
		CommandList::Init(&cmdList, cmdQueue, numParallel, numAllocators);
	}

	void _CommandList::Reset(UINT allocator, UINT index)
	{
		CommandList::Reset(&cmdList, allocator, index);
	}

	void _CommandList::Close()
	{
		CommandList::Close(&cmdList);
	}

}