module;

#include "GraphicsHeader.h"

export module TR.Graphics.DescriptorHeap;

export import TR.Graphics.Device;

export namespace TR::Graphics {

	namespace DescriptorHeap {

		struct E_FailedDescriptorHeapCreation : public _D3D12Exception {
			E_FailedDescriptorHeapCreation(HRESULT returnValue) : _D3D12Exception("Failed to create descriptor heap.", returnValue) {}
		};

		struct _Context {
			ComPtr<ID3D12DescriptorHeap> heap = {};
			UINT handleIncrementSize = 0;
		};

		void Init(_Context* descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		_NODISCARD D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(_Context* descriptorHeap, UINT index);

	}

	struct _DescriptorHeap {

		DescriptorHeap::_Context descriptorHeap = {};

		void Init(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		_NODISCARD D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(UINT index);

	};

}