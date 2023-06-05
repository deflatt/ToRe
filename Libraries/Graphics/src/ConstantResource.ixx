module;

#include "GraphicsHeader.h"

export module TR.Graphics.ConstantResource;

export import TR.Graphics.UploadResource;
export import TR.Graphics.DescriptorHeap;

export namespace TR::Graphics {

	// Resource::Constant ?
	namespace ConstantResource {

		struct _Context {
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		};

		void Init(_Context* context, Resource::_Context* resource);

		void Place(_Context* context, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	struct _ConstantBuffer {

		void Init(UINT64 size);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);
		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	protected:

		_BufferResource resource = {};
		ConstantResource::_Context context = {};

	};

	/*struct _ConstantResource {

		void Init(D3D12_RESOURCE_DESC desc);

		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	protected:

		_Resource resource = {};
		ConstantResource::_Context context = {};

	};*/

}