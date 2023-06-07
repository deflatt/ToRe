module;

#include "GraphicsHeader.h"

export module TR.Graphics.RWArrayResource;

export import TR.Graphics.BufferResource;
export import TR.Graphics.DescriptorHeap;

export namespace TR::Graphics {

	namespace RWArrayResource {

		struct _Context {
			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			UINT numElements = 0, elementSize = 0;
		};

		void Init(_Context* rwArrayResource, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, UINT numElements, UINT elementSize);

		void Place(_Context* rwArrayResource, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	namespace RWArrayResource {

		void InitBuffer(Resource::_Context* resource, Resource::Upload::_Context* upload, RWArrayResource::_Context* rwArrayResource
			, UINT numElements, UINT elementSize);

	}

	struct _RWArrayBuffer {

		Resource::_Context resource = {};
		Resource::Upload::_Context upload = {};
		RWArrayResource::_Context rwArrayResource = {};

		void Init(UINT numElements, UINT elementSize);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);
		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	};

}