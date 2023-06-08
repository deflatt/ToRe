module;

#include "GraphicsHeader.h"

module TR.Graphics.CommandAllocator;

namespace TR::Graphics {

	namespace CommandAllocator {

		void Init(_Context* cmdAllocator)
		{
			HRESULT ret;

			ret = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT
				, __uuidof(ID3D12CommandAllocator), &cmdAllocator->cmdAllocator);
			if (ret != 0) {
				throw E_FailedCommandAllocatorCreation(ret);
			}

			ret = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), &cmdAllocator->fence);
			if (ret != 0) {
				throw E_FailedFenceCreation(ret);
			}

			cmdAllocator->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (!cmdAllocator->fenceEvent) {
				throw E_FailedFenceEventCreation();
			}
		}

		void Signal(_Context* cmdAllocator, ID3D12CommandQueue* cmdQueue)
		{
			cmdAllocator->fenceTarget++;
			HRESULT ret = cmdQueue->Signal(cmdAllocator->fence.Get(), cmdAllocator->fenceTarget);
			if (ret != 0) {
				throw E_FailedFenceSignaling(ret);
			}
		}

		void Wait(_Context* cmdAllocator)
		{
			if (cmdAllocator->fence->GetCompletedValue() < cmdAllocator->fenceTarget) {
				HRESULT ret = cmdAllocator->fence->SetEventOnCompletion(cmdAllocator->fenceTarget, cmdAllocator->fenceEvent);
				if (ret != 0) {
					throw E_FailedFenceWaiting(ret);
				}
				WaitForSingleObject(cmdAllocator->fenceEvent, INFINITE);
			}
		}

		void Reset(_Context* cmdAllocator)
		{
			HRESULT ret = cmdAllocator->cmdAllocator->Reset();
			if (ret != 0) {
				throw E_FailedCommandAllocatorReset(ret);
			}
		}

		void Release(_Context* cmdAllocator)
		{
			if (cmdAllocator->fenceEvent) {
				CloseHandle(cmdAllocator->fenceEvent);
				cmdAllocator->fenceEvent = nullptr;
			}
			cmdAllocator->fenceTarget = 0;
		}

	}

	void _CommandAllocator::Init()
	{
		CommandAllocator::Init(&cmdAllocator);
	}

	void _CommandAllocator::Signal(ID3D12CommandQueue* cmdQueue)
	{
		CommandAllocator::Signal(&cmdAllocator, cmdQueue);
	}

	void _CommandAllocator::Wait()
	{
		CommandAllocator::Wait(&cmdAllocator);
	}

	void _CommandAllocator::Reset()
	{
		CommandAllocator::Reset(&cmdAllocator);
	}

	_CommandAllocator::~_CommandAllocator()
	{
		CommandAllocator::Release(&cmdAllocator);
	}

}
