module TR.Graphics.RenderTarget;

import TR.Graphics.Device;
import TR.Graphics.Definitions;
import <d3d12.h>;

namespace TR {

	namespace Graphics {
		
		namespace RenderTarget {

			void InitRenderTarget(_Resource* resource, Int2 size, DXGI_FORMAT format)
			{
				D3D12_RESOURCE_DESC desc = {};
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				desc.Alignment = 0;
				desc.DepthOrArraySize = 1;
				desc.MipLevels = 1;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
				desc.Width = size[0];
				desc.Height = size[1];
				desc.Format = format;

				Init(resource, D3D12_HEAP_TYPE_DEFAULT, desc, D3D12_RESOURCE_STATE_PRESENT, D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAG_ALLOW_DISPLAY);
			}

			void PlaceRenderTarget(_Resource* resource, _ResourceHeap* resourceHeap, UINT index)
			{
				D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = ((CD3DX12_CPU_DESCRIPTOR_HANDLE)resourceHeap->heap->GetCPUDescriptorHandleForHeapStart()).Offset(index, resourceHeap->handleIncrementSize);
				device->CreateRenderTargetView(resource->resource.Get(), nullptr, cpuHandle);
			}

		}

		namespace RenderTargetHeap {

			void Init(_RenderTargetList* rtList, _ResourceHeap* resourceHeap, UINT numRenderTargets)
			{
				Init(resourceHeap, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numRenderTargets);
				rtList->renderTargets.resize(numRenderTargets);
			}

			void Init(_RenderTargetList* rtList, _ResourceHeap* resourceHeap, UINT numRenderTargets, Int2 size, DXGI_FORMAT format)
			{
				Init(rtList, resourceHeap, numRenderTargets);
				for (UINT i = 0; i < numRenderTargets; i++) {
					InitRenderTarget(&rtList->renderTargets[i], size, format);
				}
			}

			void PlaceTargets(_RenderTargetList* rtList, _ResourceHeap* resourceHeap)
			{
				for (UINT i = 0; i < rtList->renderTargets.size(); i++) {
					PlaceRenderTarget(&rtList->renderTargets[i], resourceHeap, i);
				}
				SetFrameIndex(rtList, resourceHeap, 0);
			}

			void SetFrameIndex(_RenderTargetList* rtList, _ResourceHeap* resourceHeap, UINT frameIndex)
			{
				rtList->frameIndex = frameIndex;
				rtList->currentHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(resourceHeap->heap->GetCPUDescriptorHandleForHeapStart(), 
					frameIndex, resourceHeap->handleIncrementSize);
			}

			void NextFrame(_RenderTargetList* rtList, _ResourceHeap* resourceHeap)
			{
				SetFrameIndex(rtList, resourceHeap, (rtList->frameIndex + 1) % rtList->renderTargets.size());
			}

			void SetCurrent(_RenderTargetList* rtList, ID3D12GraphicsCommandList* cmdList)
			{
				cmdList->OMSetRenderTargets(1, &rtList->currentHandle, FALSE, nullptr);
			}

			void ClearCurrent(_RenderTargetList* rtList, ID3D12GraphicsCommandList* cmdList, Float4 color)
			{
				cmdList->ClearRenderTargetView(rtList->currentHandle, &color[0], 0, nullptr);
			}

			void Init(_RenderTargetHeap* rtHeap, UINT numRenderTargets)
			{
				Init(&rtHeap->rtList, &rtHeap->heap, numRenderTargets);
			}

			void Init(_RenderTargetHeap* rtHeap, UINT numRenderTargets, Int2 size, DXGI_FORMAT format)
			{
				Init(&rtHeap->rtList, &rtHeap->heap, numRenderTargets, size, format);
			}

			void PlaceTargets(_RenderTargetHeap* rtHeap)
			{
				PlaceTargets(&rtHeap->rtList, &rtHeap->heap);
			}

			void SetFrameIndex(_RenderTargetHeap* rtHeap, UINT frameIndex)
			{
				SetFrameIndex(&rtHeap->rtList, &rtHeap->heap, frameIndex);
			}

			void NextFrame(_RenderTargetHeap* rtHeap)
			{
				NextFrame(&rtHeap->rtList, &rtHeap->heap);
			}

			void SetCurrent(_RenderTargetHeap* rtHeap, ID3D12GraphicsCommandList* cmdList)
			{
				SetCurrent(&rtHeap->rtList, cmdList);
			}

			void ClearCurrent(_RenderTargetHeap* rtHeap, ID3D12GraphicsCommandList* cmdList, Float4 color)
			{
				ClearCurrent(&rtHeap->rtList, cmdList, color);
			}

		}

	}

}