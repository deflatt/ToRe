module;

#include "GraphicsHeader.h"

module TR.Graphics.CommandQueue;

namespace TR::Graphics {

	namespace CommandQueue {

		void Init(_Context* context)
		{
			D3D12_COMMAND_QUEUE_DESC desc = {};
			HRESULT ret = device->CreateCommandQueue(&desc, __uuidof(ID3D12CommandQueue), &context->cmdQueue);
			if (ret != 0) {
				throw E_FailedCommandQueueCreation(ret);
			}
		}

		void Execute(_Context* context, ID3D12CommandList* const* lists, UINT numLists)
		{
			context->cmdQueue->ExecuteCommandLists(numLists, lists);
		}

		void Execute(_Context* context, ID3D12CommandList* list)
		{
			ID3D12CommandList* cmdLists[1] = { list };
			Execute(context, cmdLists, 1);
		}

	}

	void _CommandQueue::Init()
	{
		CommandQueue::Init(&context);
	}

	void _CommandQueue::Execute(ID3D12CommandList* const* lists, UINT numLists)
	{
		CommandQueue::Execute(&context, lists, numLists);
	}

	void _CommandQueue::Execute(ID3D12CommandList* list)
	{
		CommandQueue::Execute(&context, list);
	}

	CommandQueue::_Context* _CommandQueue::GetContext() noexcept
	{
		return &context;
	}

}