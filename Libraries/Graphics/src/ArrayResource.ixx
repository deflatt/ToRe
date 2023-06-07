module;

#include "GraphicsHeader.h"

export module TR.Graphics.ArrayResource;

export import TR.Graphics.BufferResource;
export import TR.Graphics.DescriptorHeap;

export namespace TR::Graphics {

	namespace ArrayResource {

		struct _Context {
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			UINT numElements = 0, elementSize = 0;
		};

		void Init(_Context* arrayResource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, UINT numElements, UINT elementSize);

		void Place(_Context* arrayResource, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	namespace ArrayResource {

		void InitBuffer(Resource::_Context* resource, Resource::Upload::_Context* upload, ArrayResource::_Context* arrayResource
			, UINT numElements, UINT elementSize);

	}

	struct _ArrayBuffer {

		Resource::_Context resource = {};
		Resource::Upload::_Context upload = {};
		ArrayResource::_Context arrayResource = {};

		void Init(UINT numElements, UINT elementSize);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);
		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	};


#if false

	namespace ArrayResource {

		struct _Context {
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			UINT numElements = 0, elementSize = 0;
		};

		void Init(_Context* context, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, UINT numElements, UINT elementSize);

		void Place(_Context* context, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	namespace ArrayResource {

		struct _BufferContext {
			_Context arrayResource = {};
			_BufferResource bufferResource = {};
		};

		void Init(_BufferContext* context)

	}

	struct _ArrayBuffer { // Rewrite in procedural

		void Init(UINT numElements, UINT elementSize);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);
		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	protected:

		_BufferResource resource = {};
		ArrayResource::_Context context = {};

	};

	namespace RWArrayResource {

		struct _Context {
			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			UINT numElements = 0, elementSize = 0;
		};

		void Init(_Context* context, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, UINT numElements, UINT elementSize);

		void Place(_Context* context, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	struct _RWArrayBuffer {

		void Init(UINT numElements, UINT elementSize);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);
		void Place(DescriptorHeap::_Context* descriptorHeap, UINT index);

	protected:

		_BufferResource resource = {};
		RWArrayResource::_Context context = {};

	};

#endif

}