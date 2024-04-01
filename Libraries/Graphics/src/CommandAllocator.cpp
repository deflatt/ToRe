module TR.Graphics.CommandAllocator;

import TR.Graphics.Device;
import <d3d12.h>;

namespace TR {

	namespace Graphics {

		namespace CommandAllocator {

			void Init(_CommandAllocator* cmdAllocator)
			{
				HRESULT ret;

				ret = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT
					, __uuidof(ID3D12CommandAllocator), &cmdAllocator->cmdAllocator);
				if (ret != 0) {
					throw _FailedCommandAllocatorCreationException(ret);
				}

				ret = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), &cmdAllocator->fence);
				if (ret != 0) {
					throw _FailedFenceCreationException(ret);
				}

				cmdAllocator->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (!cmdAllocator->fenceEvent) {
					throw _FailedFenceEventCreationException();
				}
			}

			void Signal(_CommandAllocator* cmdAllocator, ID3D12CommandQueue* cmdQueue)
			{
				cmdAllocator->fenceTarget++;
				HRESULT ret = cmdQueue->Signal(cmdAllocator->fence.Get(), cmdAllocator->fenceTarget);
				if (ret != 0) {
					throw _FailedFenceSignalingException(ret);
				}
			}

			void Wait(_CommandAllocator* cmdAllocator)
			{
				if (cmdAllocator->fence->GetCompletedValue() < cmdAllocator->fenceTarget) {
					HRESULT ret = cmdAllocator->fence->SetEventOnCompletion(cmdAllocator->fenceTarget, cmdAllocator->fenceEvent);
					if (ret != 0) {
						throw _FailedFenceWaitingException(ret);
					}
					WaitForSingleObject(cmdAllocator->fenceEvent, INFINITE);
				}
			}

			void Reset(_CommandAllocator* cmdAllocator)
			{
				HRESULT ret = cmdAllocator->cmdAllocator->Reset();
				if (ret != 0) {
					throw _FailedCommandAllocatorResetException(ret);
				}
			}

			void Release(_CommandAllocator* cmdAllocator)
			{
				if (cmdAllocator->fenceEvent) {
					CloseHandle(cmdAllocator->fenceEvent);
					cmdAllocator->fenceEvent = nullptr;
				}
				cmdAllocator->fenceTarget = 0;
			}

		}

	}

}