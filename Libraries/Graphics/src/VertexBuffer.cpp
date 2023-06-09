module;

#include "GraphicsHeader.h"

module TR.Graphics.VertexBuffer;

namespace TR::Graphics {

	namespace VertexBuffer {

		void Init(_Context* vertexBuffer, Resource::_Context* resource, UINT numVertices, UINT vertexSize)
		{
			vertexBuffer->vbView.BufferLocation = resource->resource->GetGPUVirtualAddress();
			vertexBuffer->vbView.SizeInBytes = numVertices * vertexSize;
			vertexBuffer->vbView.StrideInBytes = vertexSize;
		}

		void Init(Resource::_Context* resource, Resource::Upload::_Context* upload, _Context* vertexBuffer, UINT numVertices, UINT vertexSize)
		{
			Resource::InitBuffer(resource, upload, (UINT64)numVertices * (UINT64)vertexSize);
			VertexBuffer::Init(vertexBuffer, resource, numVertices, vertexSize);
		}

		void Set(_Context* vertexBuffer, ID3D12GraphicsCommandList* cmdList)
		{
			cmdList->IASetVertexBuffers(0, 1, &vertexBuffer->vbView);
		}

	}

	void _VertexBuffer::Init(UINT numVertices, UINT vertexSize)
	{
		VertexBuffer::Init(&resource, &upload, &vertexBuffer, numVertices, vertexSize);
	}

	void _VertexBuffer::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::UploadBuffer(&upload, &resource, data, cmdList);
	}

	void _VertexBuffer::Set(ID3D12GraphicsCommandList* cmdList)
	{
		VertexBuffer::Set(&vertexBuffer, cmdList);
	}


}