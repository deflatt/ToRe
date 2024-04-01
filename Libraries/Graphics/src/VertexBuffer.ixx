export module TR.Graphics.VertexBuffer;

import TR.Graphics.BufferResource;
import TR.Graphics.Resource;
import TR.Graphics.UploadResource;
import <d3d12.h>;

export namespace TR {

	namespace Graphics {

		namespace VertexBuffer {

			struct _VertexBufferPart {
				D3D12_VERTEX_BUFFER_VIEW vbView = {};
			};

			void Init(_VertexBufferPart* vertexBuffer, _Resource* resource, UINT numVertices, UINT vertexSize);
			void Init(_Resource* resource, _UploadPart* upload, _VertexBufferPart* vertexBuffer, UINT numVertices, UINT vertexSize);

			void Set(_VertexBufferPart* vertexBuffer, ID3D12GraphicsCommandList* cmdList);

			struct _VertexBuffer {
				_Resource resource = {};
				_UploadPart upload = {};
				_VertexBufferPart vertexBuffer = {};
			};

			void Init(_VertexBuffer* vertexBuffer, UINT numVertices, UINT vertexSize);
			void Upload(_VertexBuffer* vertexBuffer, const void* data, ID3D12GraphicsCommandList* cmdList);
			void Set(_VertexBuffer* vertexBuffer, ID3D12GraphicsCommandList* cmdList);

		}
		using namespace VertexBuffer;

	}
	using namespace Graphics;

}