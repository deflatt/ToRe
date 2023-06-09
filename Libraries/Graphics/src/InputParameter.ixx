module;

#include "GraphicsHeader.h"

export module TR.Graphics.InputParameter;

export import TR.Graphics.Device;
export import TR.Graphics.DescriptorHeap;
export import <vector>;
export import <unordered_map>;

export namespace TR::Graphics {

	namespace InputParameter {

		struct _TableParameter {
			D3D12_DESCRIPTOR_RANGE_TYPE rangeType;
			UINT shaderRegister;
		};

		// Not supported yet
		struct _Constant {
			UINT shaderRegister = 0;
			UINT num32BitValues = 0;
			D3D12_ROOT_PARAMETER rootParameter = {};
		};
		struct _ConstantResource {
			UINT shaderRegister = 0;
			D3D12_ROOT_PARAMETER rootParameter = {};
			D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = {};
		};
		struct _ArrayResource {
			UINT shaderRegister = 0;
			D3D12_ROOT_PARAMETER rootParameter = {};
			D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = {};
		};
		struct _RWArrayResource {
			UINT shaderRegister = 0;
			D3D12_ROOT_PARAMETER rootParameter = {};
			D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = {};
		};
		struct _Table {
			std::vector<D3D12_DESCRIPTOR_RANGE> ranges = {};
			D3D12_ROOT_PARAMETER rootParameter = {};
			DescriptorHeap::_Context* descriptorHeap = nullptr;
		};

		void Init(_Constant* constant, UINT shaderRegister, UINT num32BitValues);
		void Init(_ConstantResource* constantResource, UINT shaderRegister);
		void Init(_ArrayResource* arrayResource, UINT shaderRegister);
		void Init(_RWArrayResource* rwArrayResource, UINT shaderRegister);
		void Init(_Table* table, std::vector<_TableParameter> parameters);

		//void SetParameter(_Constant* constant, ID3D12GraphicsCommandList* cmdList, UINT index);
		void SetParameter(_ConstantResource* constantResource, ID3D12GraphicsCommandList* cmdList, UINT index);
		void SetParameter(_ArrayResource* arrayResource, ID3D12GraphicsCommandList* cmdList, UINT index);
		void SetParameter(_RWArrayResource* rwArrayResource, ID3D12GraphicsCommandList* cmdList, UINT index);
		void SetParameter(_Table* table, ID3D12GraphicsCommandList* cmdList, UINT index);

		namespace Map {

			struct _Context {
				std::unordered_map<std::string, _Constant> constants = {};
				std::unordered_map<std::string, _ConstantResource> constantResources = {};
				std::unordered_map<std::string, _ArrayResource> arrayResources = {};
				std::unordered_map<std::string, _RWArrayResource> rwArrayResources = {};
				std::unordered_map<std::string, _Table> tables = {};
			};

			void FillParameters(_Context* map, std::vector<D3D12_ROOT_PARAMETER>* rootParameters);
			void SetParameters(_Context* map, ID3D12GraphicsCommandList* cmdList);

		}

	}

	struct _InputParameterMap {

		InputParameter::Map::_Context map = {};

		void FillParameters(std::vector<D3D12_ROOT_PARAMETER>* rootParameters);
		void SetParameters(ID3D12GraphicsCommandList* cmdList);

	};

}