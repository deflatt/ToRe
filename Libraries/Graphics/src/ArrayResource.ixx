export module TR.Graphics.ArrayResource;

import TR.Graphics.UploadResource;
import TR.Graphics.Definitions;
import TR.Graphics.Exception;
import TR.Graphics.ResourceHeap;
import TR.Graphics.BufferResource;
import <d3d12.h>;

export namespace TR {

	namespace Graphics {

		namespace ArrayResource {

			struct _ArrayPart {
				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				UINT numElements = 0, elementSize = 0;
			};

			void Init(_ArrayPart* array, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, UINT numElements, UINT elementSize);

			void Place(_ArrayPart* array, _Resource* resource, _ResourceHeap* resourceHeap, UINT index);

			void InitBuffer(_Resource* resource, _UploadPart* upload, _ArrayPart* array
				, UINT numElements, UINT elementSize);

			struct _ArrayBuffer {
				_Resource resource = {};
				_UploadPart upload = {};
				_ArrayPart array = {};
			};

			void Init(_ArrayBuffer* arrayBuffer, UINT numElements, UINT elementSize);

			struct _RWArrayPart {
				D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				UINT numElements = 0, elementSize = 0;
			};

			void Init(_RWArrayPart* rwArray, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, UINT numElements, UINT elementSize);
			void Place(_RWArrayPart* rwArray, _Resource* resource, _ResourceHeap* resourceHeap, UINT index);

			struct _RWArrayBuffer {
				_Resource resource = {};
				_UploadPart upload = {};
				_RWArrayPart rwArray = {};
			};



		}
		using namespace ArrayResource;

	}
	using namespace Graphics;

}