module;

#include "GraphicsHeader.h"

module TR.Graphics.CommandAllocator;

namespace TR::Graphics {

	namespace CommandAllocator {

		void Init(_Context* context)
		{
			HRESULT ret;

			ret = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT
				, __uuidof(ID3D12CommandAllocator), &context->cmdAllocator);
			if (ret != 0) {
				throw E_FailedCommandAllocatorCreation(ret);
			}

			ret = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), &context->fence);
			if (ret != 0) {
				throw E_FailedFenceCreation(ret);
			}

			context->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (!context->fenceEvent) {
				throw E_FailedFenceEventCreation();
			}
		}

		void Signal(_Context* context, ID3D12CommandQueue* cmdQueue)
		{
			context->fenceTarget++;
			HRESULT ret = cmdQueue->Signal(context->fence.Get(), context->fenceTarget);
			if (ret != 0) {
				throw E_FailedFenceSignaling(ret);
			}
		}

		void Wait(_Context* context)
		{
			if (context->fence->GetCompletedValue() < context->fenceTarget) {
				HRESULT ret = context->fence->SetEventOnCompletion(context->fenceTarget, context->fenceEvent);
				if (ret != 0) {
					throw E_FailedFenceWaiting(ret);
				}
				WaitForSingleObject(context->fenceEvent, INFINITE);
			}
		}

		void Reset(_Context* context)
		{
			HRESULT ret = context->cmdAllocator->Reset();
			if (ret != 0) {
				throw E_FailedCommandAllocatorReset(ret);
			}
		}

		void Release(_Context* context)
		{
			if (context->fenceEvent) {
				CloseHandle(context->fenceEvent);
				context->fenceEvent = nullptr;
			}
			context->fenceTarget = 0;
		}

	}

	void _CommandAllocator::Init()
	{
		CommandAllocator::Init(&context);
	}

	void _CommandAllocator::Signal(ID3D12CommandQueue* cmdQueue)
	{
		CommandAllocator::Signal(&context, cmdQueue);
	}

	void _CommandAllocator::Wait()
	{
		CommandAllocator::Wait(&context);
	}

	void _CommandAllocator::Reset()
	{
		CommandAllocator::Reset(&context);
	}

	_CommandAllocator::~_CommandAllocator()
	{
		CommandAllocator::Release(&context);
	}

}
