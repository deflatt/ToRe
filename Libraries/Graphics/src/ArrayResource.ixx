module;

#include "GraphicsHeader.h"

export module TR.Graphics.ArrayResource;

export import TR.Graphics.UploadResource;
export import TR.Graphics.DescriptorHeap;

export namespace TR::Graphics {

	namespace ArrayResource {

		struct _Context {
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			UINT numElements = 0, elementSize = 0;
		};

		void Init(_Context* context, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, UINT numElements, UINT elementSize);

		void Place(_Context* context, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index);

	}

	struct _ArrayBuffer {

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

}