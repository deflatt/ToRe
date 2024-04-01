module TR.Graphics.VertexBuffer;

namespace TR {

	namespace Graphics {

		namespace VertexBuffer {

			void Init(_VertexBufferPart* vertexBuffer, _Resource* resource, UINT numVertices, UINT vertexSize)
			{
				vertexBuffer->vbView.BufferLocation = resource->resource->GetGPUVirtualAddress();
				vertexBuffer->vbView.SizeInBytes = numVertices * vertexSize;
				vertexBuffer->vbView.StrideInBytes = vertexSize;
			}

			void Init(_Resource* resource, _UploadPart* upload, _VertexBufferPart* vertexBuffer, UINT numVertices, UINT vertexSize)
			{
				InitBuffer(resource, upload, (UINT64)numVertices * (UINT64)vertexSize);
				Init(vertexBuffer, resource, numVertices, vertexSize);
			}

			void Upload(_VertexBuffer* vertexBuffer, const void* data, ID3D12GraphicsCommandList* cmdList)
			{
				UploadBuffer(&vertexBuffer->upload, &vertexBuffer->resource, data, cmdList);
			}

			void Set(_VertexBufferPart* vertexBuffer, ID3D12GraphicsCommandList* cmdList)
			{
				cmdList->IASetVertexBuffers(0, 1, &vertexBuffer->vbView);
			}

			void Init(_VertexBuffer* vertexBuffer, UINT numVertices, UINT vertexSize)
			{
				Init(&vertexBuffer->resource, &vertexBuffer->upload, &vertexBuffer->vertexBuffer, numVertices, vertexSize);
			}

			void Set(_VertexBuffer* vertexBuffer, ID3D12GraphicsCommandList* cmdList)
			{
				Set(&vertexBuffer->vertexBuffer, cmdList);
			}

		}

	}

}