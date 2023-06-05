module;

#include "GraphicsHeader.h"

export module TR.Graphics.RenderTarget;

export import TR.Graphics.Resource;
export import TR.Graphics.DescriptorHeap;
export import TR.Essentials.Array;

export import <vector>;

export namespace TR::Graphics {

	namespace RenderTarget {

		void Init(Resource::_Context* resource, Long2 size, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void Place(Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	struct _RenderTarget {

		void Init(Long2 size, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

		_NODISCARD Resource::_Context* GetResource() noexcept;

	protected:

		Resource::_Context resource = {};

	};

	namespace RenderTargetHeap {

		struct _Context {
			_DescriptorHeap heap = {};
			std::vector<_RenderTarget> renderTargets = {};
			UINT frameIndex = 0;
			CD3DX12_CPU_DESCRIPTOR_HANDLE currentHandle = {};
		};

		void Init(_Context* context, UINT numRenderTargets);
		void Init(_Context* context, UINT numRenderTargets, Long2 size, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void PlaceTargets(_Context* context);

		void SetFrameIndex(_Context* context, UINT frameIndex);
		void NextFrame(_Context* context);

		void SetCurrent(_Context* context, ID3D12GraphicsCommandList* cmdList);
		void ClearCurrent(_Context* context, ID3D12GraphicsCommandList* cmdList, Float4 color);

	}

	struct _RenderTargetHeap {

		void Init(UINT numRenderTargets);
		void Init(UINT numRenderTargets, Long2 size, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void PlaceTargets();

		void SetFrameIndex(UINT frameIndex);

		void SetCurrent(ID3D12GraphicsCommandList* cmdList);
		void ClearCurrent(ID3D12GraphicsCommandList* cmdList, Float4 color);

		_NODISCARD RenderTargetHeap::_Context* GetContext() noexcept;

	protected:

		RenderTargetHeap::_Context context = {};

	};

}