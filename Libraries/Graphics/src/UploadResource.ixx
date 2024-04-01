export module TR.Graphics.UploadResource;

export import TR.Graphics.Resource;
import TR.Graphics.Definitions;
import TR.Graphics.Exception;
import <d3d12.h>;

export namespace TR {

	namespace Graphics {

		namespace UploadResource {

			struct _UploadPart {
				_Resource uploadResource = {};
			};

			void Init(_UploadPart* upload, D3D12_RESOURCE_DESC desc, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);
			void Init(_UploadPart* upload, _Resource* dstResource, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void UploadData(_UploadPart* upload, _Resource* dstResource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

			struct _UploadResource {
				_Resource resource = {};
				_UploadPart upload = {};
			};

			void Init(_Resource* resource, _UploadPart* upload, D3D12_RESOURCE_DESC desc
				, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
				, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void Init(_UploadResource* uploadResource, D3D12_RESOURCE_DESC desc
				, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
				, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		}
		using namespace UploadResource;

	}
	using namespace Graphics;

}