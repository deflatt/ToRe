module;

#include "GraphicsHeader.h"

module TR.Graphics.InputParameter;

import TR.Graphics.Device;

namespace TR::Graphics {

	namespace InputParameter {

		void Init(_Constant* constant, UINT shaderRegister, UINT num32BitValues)
		{
			constant->shaderRegister = shaderRegister;
			constant->num32BitValues = num32BitValues;
			constant->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			constant->rootParameter.Constants.ShaderRegister = constant->shaderRegister;
			constant->rootParameter.Constants.RegisterSpace = 0;
			constant->rootParameter.Constants.Num32BitValues = constant->num32BitValues;
		}

		void Init(_ConstantResource* constantResource, UINT shaderRegister)
		{
			constantResource->shaderRegister = shaderRegister;
			constantResource->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			constantResource->rootParameter.Descriptor.ShaderRegister = constantResource->shaderRegister;
			constantResource->rootParameter.Descriptor.RegisterSpace = 0;
		}

		void Init(_ArrayResource* arrayResource, UINT shaderRegister)
		{
			arrayResource->shaderRegister = shaderRegister;
			arrayResource->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
			arrayResource->rootParameter.Descriptor.ShaderRegister = arrayResource->shaderRegister;
			arrayResource->rootParameter.Descriptor.RegisterSpace = 0;
		}

		void Init(_RWArrayResource* rwArrayResource, UINT shaderRegister)
		{
			rwArrayResource->shaderRegister = shaderRegister;
			rwArrayResource->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
			rwArrayResource->rootParameter.Descriptor.ShaderRegister = rwArrayResource->shaderRegister;
			rwArrayResource->rootParameter.Descriptor.RegisterSpace = 0;
		}

		void Init(_Table* table, std::vector<_TableParameter> parameters)
		{
			for (UINT i = 0; i < parameters.size(); i++) {
				if (table->ranges.size() == 0 || !(parameters[i].rangeType == parameters[i - 1].rangeType
					&& parameters[i].shaderRegister == (parameters[i - 1].shaderRegister + 1))) {

					D3D12_DESCRIPTOR_RANGE range = {};
					range.BaseShaderRegister = parameters[i].shaderRegister;
					range.NumDescriptors = 1;
					range.OffsetInDescriptorsFromTableStart = 0;
					range.RangeType = parameters[i].rangeType;

					table->ranges.push_back(range);
				}
				else {
					table->ranges.back().NumDescriptors++;
				}
			}
			table->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			table->rootParameter.DescriptorTable.NumDescriptorRanges = (UINT)table->ranges.size();
			table->rootParameter.DescriptorTable.pDescriptorRanges = &table->ranges[0];
		}

		void SetParameter(_ConstantResource* constantResource, ID3D12GraphicsCommandList* cmdList, UINT index)
		{ cmdList->SetGraphicsRootConstantBufferView(index, constantResource->gpuAddress); }

		void SetParameter(_ArrayResource* arrayResource, ID3D12GraphicsCommandList* cmdList, UINT index)
		{ cmdList->SetGraphicsRootShaderResourceView(index, arrayResource->gpuAddress); }

		void SetParameter(_RWArrayResource* rwArrayResource, ID3D12GraphicsCommandList* cmdList, UINT index)
		{ cmdList->SetGraphicsRootUnorderedAccessView(index, rwArrayResource->gpuAddress); }

		void SetParameter(_Table* table, ID3D12GraphicsCommandList* cmdList, UINT index)
		{
			ID3D12DescriptorHeap* heaps[1] = { table->descriptorHeap->heap.Get() };
			cmdList->SetDescriptorHeaps(1, heaps);
			cmdList->SetGraphicsRootDescriptorTable(index, table->descriptorHeap->heap->GetGPUDescriptorHandleForHeapStart());
		}

		namespace Map {

			void Init(_Context* map)
			{
				HRESULT ret;

				//for (auto& constant : map->constants)
				//	map->rootParameters.push_back(constant.second.rootParameter);
				for (auto& constantResource : map->constantResources) {
					Init(&constantResource.second, constantResource.second.shaderRegister);
					map->rootParameters.push_back(constantResource.second.rootParameter);
				}
				for (auto& arrayResource : map->arrayResources) {
					Init(&arrayResource.second, arrayResource.second.shaderRegister);
					map->rootParameters.push_back(arrayResource.second.rootParameter);
				}
				for (auto& rwArrayResource : map->rwArrayResources) {
					Init(&rwArrayResource.second, rwArrayResource.second.shaderRegister);
					map->rootParameters.push_back(rwArrayResource.second.rootParameter);
				}
				for (auto& table : map->tables) {
					map->rootParameters.push_back(table.second.rootParameter);
				}

				CD3DX12_ROOT_SIGNATURE_DESC rootDesc = {};
				rootDesc.Init((UINT)map->rootParameters.size(), &map->rootParameters[0], (UINT)map->staticSamplers.size()
					, &map->staticSamplers[0], D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

				ComPtr<ID3DBlob> signatureBlob = {};
				ret = D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, nullptr);
				if (ret != 0) {
					throw E_FailedRootSignatureSerialization(ret);
				}

				ret = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), &map->rootSignature);
				if (ret != 0) {
					throw E_FailedRootSignatureCreation(ret);
				}
			}

			void FillParameters(_Context* map, std::vector<D3D12_ROOT_PARAMETER>* rootParameters)
			{
				
			}

			void SetParameters(_Context* map, ID3D12GraphicsCommandList* cmdList)
			{
				cmdList->SetGraphicsRootSignature(map->rootSignature.Get());

				UINT index = 0;
				for (auto& constantResource : map->constantResources)
					SetParameter(&constantResource.second, cmdList, index++);
				for (auto& arrayResource : map->arrayResources)
					SetParameter(&arrayResource.second, cmdList, index++);
				for (auto& rwArrayResource : map->rwArrayResources)
					SetParameter(&rwArrayResource.second, cmdList, index++);
				for (auto& table : map->tables)
					SetParameter(&table.second, cmdList, index++);
			}

		}

	}

	void _InputParameterMap::Init()
	{
		InputParameter::Map::Init(&map);
	}

	void _InputParameterMap::SetParameters(ID3D12GraphicsCommandList* cmdList)
	{
		InputParameter::Map::SetParameters(&map, cmdList);
	}

}