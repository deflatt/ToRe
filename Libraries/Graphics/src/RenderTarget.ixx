export module TR.Graphics.RenderTarget;

import TR.Graphics.Definitions;
import TR.Graphics.Resource;
import TR.Graphics.ResourceHeap;
export import TR.Essentials.Vec;
export import <vector>;

export namespace TR {

	namespace Graphics {

		namespace RenderTarget {

			void InitRenderTarget(_Resource* resource, Int2 size, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

			void PlaceRenderTarget(_Resource* resource, _ResourceHeap* resourceHeap, UINT index);

		}
		using namespace RenderTarget;

		namespace RenderTargetHeap {

			struct _RenderTargetList {
				std::vector<_Resource> renderTargets = {};
				UINT frameIndex = 0;
				CD3DX12_CPU_DESCRIPTOR_HANDLE currentHandle = {};
			};

			void Init(_RenderTargetList* rtList, _ResourceHeap* resourceHeap, UINT numRenderTargets);
			void Init(_RenderTargetList* rtList, _ResourceHeap* resourceHeap, UINT numRenderTargets, Int2 size, DXGI_FORMAT format);

			void PlaceTargets(_RenderTargetList* rtList, _ResourceHeap* resourceHeap);

			void SetFrameIndex(_RenderTargetList* rtList, _ResourceHeap* resourceHeap, UINT frameIndex);
			void NextFrame(_RenderTargetList* rtList, _ResourceHeap* resourceHeap);

			void SetCurrent(_RenderTargetList* rtList, ID3D12GraphicsCommandList* cmdList);
			void ClearCurrent(_RenderTargetList* rtList, ID3D12GraphicsCommandList* cmdList, Float4 color);

			struct _RenderTargetHeap {
				_ResourceHeap heap = {};
				_RenderTargetList rtList = {};
			};

			void Init(_RenderTargetHeap* rtHeap, UINT numRenderTargets);
			void Init(_RenderTargetHeap* rtHeap, UINT numRenderTargets, Int2 size, DXGI_FORMAT format);

			void PlaceTargets(_RenderTargetHeap* rtHeap);

			void SetFrameIndex(_RenderTargetHeap* rtHeap, UINT frameIndex);
			void NextFrame(_RenderTargetHeap* rtHeap);

			void SetCurrent(_RenderTargetHeap* rtHeap, ID3D12GraphicsCommandList* cmdList);
			void ClearCurrent(_RenderTargetHeap* rtHeap, ID3D12GraphicsCommandList* cmdList, Float4 color);

		}
		using namespace RenderTargetHeap;

	}
	using namespace Graphics;

}