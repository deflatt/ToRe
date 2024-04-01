export module TR.Graphics.ResourceHeap;

import TR.Graphics.Exception;
import TR.Graphics.Definitions;

export namespace TR {

	namespace Graphics {

		namespace ResourceHeap {

			struct _ResourceHeap {
				ComPtr<ID3D12DescriptorHeap> heap = {};
				UINT handleIncrementSize = 0;
			};

			struct _FailedResourceHeapCreationException : public _D3D12Exception {
				_FailedResourceHeapCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create descriptor heap.", returnValue) {}
			};

			void Init(_ResourceHeap* resourceHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

			D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(_ResourceHeap* resourceHeap, UINT index);

		}
		using namespace ResourceHeap;

	}
	using namespace Graphics;

}