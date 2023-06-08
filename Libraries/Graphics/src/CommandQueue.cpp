module;

#include "GraphicsHeader.h"

module TR.Graphics.CommandQueue;

namespace TR::Graphics {

	namespace CommandQueue {

		void Init(_Context* cmdQueue)
		{
			D3D12_COMMAND_QUEUE_DESC desc = {};
			HRESULT ret = device->CreateCommandQueue(&desc, __uuidof(ID3D12CommandQueue), &cmdQueue->cmdQueue);
			if (ret != 0) {
				throw E_FailedCommandQueueCreation(ret);
			}
		}

		void Execute(_Context* cmdQueue, ID3D12CommandList* const* lists, UINT numLists)
		{
			cmdQueue->cmdQueue->ExecuteCommandLists(numLists, lists);
		}

		void Execute(_Context* cmdQueue, ID3D12CommandList* list)
		{
			ID3D12CommandList* cmdLists[1] = { list };
			Execute(cmdQueue, cmdLists, 1);
		}

	}

	void _CommandQueue::Init()
	{
		CommandQueue::Init(&cmdQueue);
	}

	void _CommandQueue::Execute(ID3D12CommandList* const* lists, UINT numLists)
	{
		CommandQueue::Execute(&cmdQueue, lists, numLists);
	}

	void _CommandQueue::Execute(ID3D12CommandList* list)
	{
		CommandQueue::Execute(&cmdQueue, list);
	}

}