export module TR.Graphics.InputParameter;

import <d3d12.h>;
import TR.Graphics.ArrayResource;
import TR.Graphics.ResourceHeap;
import TR.Graphics.Definitions;
export import TR.Graphics.Exception;
import <vector>;
import <string>;
import <unordered_map>;

export namespace TR {

	namespace Graphics {

		namespace InputParameter {
		
			struct _TableParameter {
				D3D12_DESCRIPTOR_RANGE_TYPE rangeType;
				UINT shaderRegister;
			};

			/* Not supported yet */
			struct _ConstantParameter {
				UINT shaderRegister = 0;
				UINT num32BitValues = 0;
				D3D12_ROOT_PARAMETER rootParameter = {};
			};
			struct _ConstantBufferParameter {
				UINT shaderRegister = 0;
				D3D12_ROOT_PARAMETER rootParameter = {};
				D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = {};
			};
			struct _ArrayParameter {
				UINT shaderRegister = 0;
				D3D12_ROOT_PARAMETER rootParameter = {};
				D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = {};
			};
			struct _RWArrayParameter {
				UINT shaderRegister = 0;
				D3D12_ROOT_PARAMETER rootParameter = {};
				D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = {};
			};
			struct _ParameterTable {
				std::vector<D3D12_DESCRIPTOR_RANGE> ranges = {};
				D3D12_ROOT_PARAMETER rootParameter = {};
				_ResourceHeap* resourceHeap = nullptr;
			};

			void Init(_ConstantParameter* constant, UINT shaderRegister, UINT num32BitValues);
			void Init(_ConstantBufferParameter* constant, UINT shaderRegister);
			void Init(_ArrayParameter* array, UINT shaderRegister);
			void Init(_RWArrayParameter* rwArray, UINT shaderRegister);
			void Init(_ParameterTable* table, std::vector<_TableParameter> parameters);
			
			//void SetParameter(_Constant* constant, ID3D12GraphicsCommandList* cmdList, UINT index);
			void SetParameter(_ConstantBufferParameter* constant, ID3D12GraphicsCommandList* cmdList, UINT index);
			void SetParameter(_ArrayParameter* array, ID3D12GraphicsCommandList* cmdList, UINT index);
			void SetParameter(_RWArrayParameter* rwArray, ID3D12GraphicsCommandList* cmdList, UINT index);
			void SetParameter(_ParameterTable* table, ID3D12GraphicsCommandList* cmdList, UINT index);

			struct _InputMap {
				std::unordered_map<std::string, _ConstantParameter> constants = {};
				std::unordered_map<std::string, _ConstantBufferParameter> constantBuffers = {};
				std::unordered_map<std::string, _ArrayParameter> arrays = {};
				std::unordered_map<std::string, _RWArrayParameter> rwArrays = {};
				std::unordered_map<std::string, _ParameterTable> tables = {};

				std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};
				std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = {};

				ComPtr<ID3D12RootSignature> rootSignature = {};
			};

			struct _FailedRootSignatureSerializationException : public _D3D12Exception {
				_FailedRootSignatureSerializationException(HRESULT returnValue) : _D3D12Exception("Failed to serialize root signature.", returnValue) {}
			};
			struct _FailedRootSignatureCreationException : public _D3D12Exception {
				_FailedRootSignatureCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create root signature.", returnValue) {}
			};

			void Init(_InputMap* map);
			void SetParameters(_InputMap* map, ID3D12GraphicsCommandList* cmdList);

		}
		using namespace InputParameter;

	}
	using namespace Graphics;
}