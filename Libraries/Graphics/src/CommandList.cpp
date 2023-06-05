module;

#include "GraphicsHeader.h"

module TR.Graphics.CommandList;

namespace TR::Graphics {

	namespace CommandList {

		void Init(_Context* context, ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators)
		{
			context->numParallel = numParallel;
			context->numAllocators = numAllocators;

			context->allocators.resize(numAllocators);
			for (UINT i = 0; i < numAllocators; i++) {
				context->allocators[i].resize(numParallel);
				for (UINT j = 0; j < numParallel; j++)
					context->allocators[i][j].Init();
			}

			HRESULT ret = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT
				, context->allocators[0][0].GetContext()->cmdAllocator.Get(), nullptr
				, __uuidof(ID3D12GraphicsCommandList), &context->cmdList);
			if (ret != 0) {
				throw E_FailedCommandListCreation(ret);
			}
		}

		void Reset(_Context* context, UINT allocator, UINT index)
		{
			context->allocators[allocator][index].Reset();

			HRESULT ret = context->cmdList->Reset(context->allocators[allocator][index]
				.GetContext()->cmdAllocator.Get(), nullptr);
			if (ret != 0) {
				throw E_FailedCommandListReset(ret);
			}
		}

		void Close(_Context* context)
		{
			HRESULT ret = context->cmdList->Close();
			if (ret != 0) {
				throw E_FailedCommandListClosure(ret);
			}
		}

	}

	void _CommandList::Init(ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators)
	{
		CommandList::Init(&context, cmdQueue, numParallel, numAllocators);
	}

	void _CommandList::Reset(UINT allocator, UINT index)
	{
		CommandList::Reset(&context, allocator, index);
	}

	void _CommandList::Close()
	{
		CommandList::Close(&context);
	}

	CommandList::_Context* _CommandList::GetContext() noexcept
	{
		return &context;
	}

}