module;

#include "GraphicsHeader.h"

export module TR.Graphics.VertexBuffer;

export import TR.Graphics.BufferResource;
export import TR.Graphics.DescriptorHeap;

export namespace TR::Graphics {

	namespace VertexBuffer {

		struct _Context {
			D3D12_VERTEX_BUFFER_VIEW vbView = {};
		};

		void Init(_Context* vertexBuffer, Resource::_Context* resource, UINT numVertices, UINT vertexSize);
		void Init(Resource::_Context* resource, Resource::Upload::_Context* upload, _Context* vertexBuffer, UINT numVertices, UINT vertexSize);

		void Set(_Context* vertexBuffer, ID3D12GraphicsCommandList* cmdList);

	}

	struct _VertexBuffer {

		Resource::_Context resource = {};
		Resource::Upload::_Context upload = {};
		VertexBuffer::_Context vertexBuffer = {};

		void Init(UINT numVertices, UINT vertexSize);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);
		void Set(ID3D12GraphicsCommandList* cmdList);

	};

}