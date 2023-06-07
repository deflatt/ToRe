module;

#include "GraphicsHeader.h"

export module TR.Graphics.Resource;

export import TR.Graphics.Device;

export namespace TR::Graphics {

	namespace Resource {

		struct E_FailedResourceCreation : public _D3D12Exception {
			E_FailedResourceCreation(HRESULT returnValue) : _D3D12Exception("Failed to create resource.", returnValue) {}
		};

		struct _Context {
			ComPtr<ID3D12Resource> resource = {};
			D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
		};

		void Init(_Context* context, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc
			, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void Transition(_Context* context, D3D12_RESOURCE_STATES targetState, ID3D12GraphicsCommandList* cmdList);

	}

	struct _Resource {

		Resource::_Context context = {};

		void Init(D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc
			, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void Transition(D3D12_RESOURCE_STATES targetState, ID3D12GraphicsCommandList* cmdList);

	};

}