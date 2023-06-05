module;

#include "GraphicsHeader.h"

module TR.Graphics.RenderTarget;

namespace TR::Graphics {

	namespace RenderTarget {

		void Init(Resource::_Context* resource, Long2 size, DXGI_FORMAT format)
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

			Resource::Init(resource, D3D12_HEAP_TYPE_DEFAULT, desc, D3D12_RESOURCE_STATE_PRESENT, D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAG_ALLOW_DISPLAY);
		}

		void Place(Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = ((CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap->heap->GetCPUDescriptorHandleForHeapStart()).Offset(index, descriptorHeap->handleIncrementSize);
			device->CreateRenderTargetView(resource->resource.Get(), nullptr, cpuHandle);
		}

	}

	void _RenderTarget::Init(Long2 size, DXGI_FORMAT format)
	{
		RenderTarget::Init(&resource, size, format);
	}

	void _RenderTarget::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	{
		RenderTarget::Place(&resource, descriptorHeap, index);
	}

	Resource::_Context* _RenderTarget::GetResource() noexcept
	{
		return &resource;
	}

	namespace RenderTargetHeap {

		void Init(_Context* context, UINT numRenderTargets)
		{
			context->heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numRenderTargets);
			context->renderTargets.resize(numRenderTargets);
		}

		void Init(_Context* context, UINT numRenderTargets, Long2 size, DXGI_FORMAT format)
		{
			Init(context, numRenderTargets);
			for (UINT i = 0; i < numRenderTargets; i++) {
				context->renderTargets[i].Init(size, format);
			}
		}

		void PlaceTargets(_Context* context)
		{
			for (UINT i = 0; i < context->renderTargets.size(); i++) {
				context->renderTargets[i].Place(context->heap.GetContext(), i);
			}
			SetFrameIndex(context, 0);
		}

		void SetFrameIndex(_Context* context, UINT frameIndex)
		{
			context->frameIndex = frameIndex;
			context->currentHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(context->heap.GetContext()->heap->GetCPUDescriptorHandleForHeapStart()
				, frameIndex, context->heap.GetContext()->handleIncrementSize);
		}

		void NextFrame(_Context* context)
		{
			SetFrameIndex(context, (context->frameIndex + 1) % context->renderTargets.size());
		}

		void SetCurrent(_Context* context, ID3D12GraphicsCommandList* cmdList)
		{
			cmdList->OMSetRenderTargets(1, &context->currentHandle, FALSE, nullptr);
		}

		void ClearCurrent(_Context* context, ID3D12GraphicsCommandList* cmdList, Float4 color)
		{
			cmdList->ClearRenderTargetView(context->currentHandle, &color[0], 0, nullptr);
		}

	}

	void _RenderTargetHeap::Init(UINT numRenderTargets)
	{
		RenderTargetHeap::Init(&context, numRenderTargets);
	}

	void _RenderTargetHeap::Init(UINT numRenderTargets, Long2 size, DXGI_FORMAT format)
	{
		RenderTargetHeap::Init(&context, numRenderTargets, size, format);
	}

	void _RenderTargetHeap::PlaceTargets()
	{
		RenderTargetHeap::PlaceTargets(&context);
	}

	void _RenderTargetHeap::SetFrameIndex(UINT frameIndex)
	{
		RenderTargetHeap::SetFrameIndex(&context, frameIndex);
	}

	void _RenderTargetHeap::SetCurrent(ID3D12GraphicsCommandList* cmdList)
	{
		RenderTargetHeap::SetCurrent(&context, cmdList);
	}

	void _RenderTargetHeap::ClearCurrent(ID3D12GraphicsCommandList* cmdList, Float4 color)
	{
		RenderTargetHeap::ClearCurrent(&context, cmdList, color);
	}

	RenderTargetHeap::_Context* _RenderTargetHeap::GetContext() noexcept
	{
		return &context;
	}

}