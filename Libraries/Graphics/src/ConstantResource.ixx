module;

#include "GraphicsHeader.h"

export module TR.Graphics.ConstantResource;

export import TR.Graphics.BufferResource;
export import TR.Graphics.DescriptorHeap;

export namespace TR::Graphics {

	namespace ConstantResource {

		struct _Context {
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		};

		void Init(_Context* constantResource, Resource::_Context* resource);

		void Place(_Context* constantResource, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	namespace ConstantResource {

		void InitBuffer(Resource::_Context* resource, Resource::Upload::_Context* upload, ConstantResource::_Context* constantResource
			, UINT64 numBytes);

	}

	struct _ConstantBuffer {

		Resource::_Context resource = {};
		Resource::Upload::_Context upload = {};
		ConstantResource::_Context constantResource = {};

		void Init(UINT64 numBytes);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);
		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	};

	/*struct _ConstantResource {

		void Init(D3D12_RESOURCE_DESC desc);

		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	protected:

		_Resource resource = {};
		ConstantResource::_Context context = {};

	};*/

}