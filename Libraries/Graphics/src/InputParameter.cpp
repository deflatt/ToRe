module TR.Graphics.InputParameter;

import TR.Graphics.Device;

namespace TR {

	namespace Graphics {

		namespace InputParameter {

			void Init(_ConstantParameter* constant, UINT shaderRegister, UINT num32BitValues)
			{
				constant->shaderRegister = shaderRegister;
				constant->num32BitValues = num32BitValues;
				constant->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
				constant->rootParameter.Constants.ShaderRegister = constant->shaderRegister;
				constant->rootParameter.Constants.RegisterSpace = 0;
				constant->rootParameter.Constants.Num32BitValues = constant->num32BitValues;
			}

			void Init(_ConstantBufferParameter* constant, UINT shaderRegister)
			{
				constant->shaderRegister = shaderRegister;
				constant->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				constant->rootParameter.Descriptor.ShaderRegister = shaderRegister;
				constant->rootParameter.Descriptor.RegisterSpace = 0;
			}

			void Init(_ArrayParameter* array, UINT shaderRegister)
			{
				array->shaderRegister = shaderRegister;
				array->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
				array->rootParameter.Descriptor.ShaderRegister = shaderRegister;
				array->rootParameter.Descriptor.RegisterSpace = 0;
			}

			void Init(_RWArrayParameter* rwArray, UINT shaderRegister)
			{
				rwArray->shaderRegister = shaderRegister;
				rwArray->rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
				rwArray->rootParameter.Descriptor.ShaderRegister = rwArray->shaderRegister;
				rwArray->rootParameter.Descriptor.RegisterSpace = 0;
			}

			void Init(_ParameterTable* table, std::vector<_TableParameter> parameters)
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

			void SetParameter(_ConstantBufferParameter* constant, ID3D12GraphicsCommandList* cmdList, UINT index)
			{
				cmdList->SetGraphicsRootConstantBufferView(index, constant->gpuAddress);
			}

			void SetParameter(_ArrayParameter* array, ID3D12GraphicsCommandList* cmdList, UINT index) 
			{
				cmdList->SetGraphicsRootShaderResourceView(index, array->gpuAddress);
			}

			void SetParameter(_RWArrayParameter* rwArray, ID3D12GraphicsCommandList* cmdList, UINT index)
			{
				cmdList->SetGraphicsRootUnorderedAccessView(index, rwArray->gpuAddress);
			}

			void SetParameter(_ParameterTable* table, ID3D12GraphicsCommandList* cmdList, UINT index)
			{
				ID3D12DescriptorHeap* heaps[1] = { table->resourceHeap->heap.Get() };
				cmdList->SetDescriptorHeaps(1, heaps);
				cmdList->SetGraphicsRootDescriptorTable(index, table->resourceHeap->heap->GetGPUDescriptorHandleForHeapStart());
			}

			void Init(_InputMap* map)
			{
				HRESULT ret;

				//for (auto& constant : map->constants)
				//	map->rootParameters.push_back(constant.second.rootParameter);
				for (auto& constantBuffer : map->constantBuffers) {
					Init(&constantBuffer.second, constantBuffer.second.shaderRegister);
					map->rootParameters.push_back(constantBuffer.second.rootParameter);
				}
				for (auto& array : map->arrays) {
					Init(&array.second, array.second.shaderRegister);
					map->rootParameters.push_back(array.second.rootParameter);
				}
				for (auto& rwArray : map->rwArrays) {
					Init(&rwArray.second, rwArray.second.shaderRegister);
					map->rootParameters.push_back(rwArray.second.rootParameter);
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
					throw _FailedRootSignatureSerializationException(ret);
				}

				ret = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), &map->rootSignature);
				if (ret != 0) {
					throw _FailedRootSignatureCreationException(ret);
				}
			}

			void SetParameters(_InputMap* map, ID3D12GraphicsCommandList* cmdList)
			{
				cmdList->SetGraphicsRootSignature(map->rootSignature.Get());

				UINT index = 0;
				for (auto& constantBuffer : map->constantBuffers)
					SetParameter(&constantBuffer.second, cmdList, index++);
				for (auto& array : map->arrays)
					SetParameter(&array.second, cmdList, index++);
				for (auto& rwArray : map->rwArrays)
					SetParameter(&rwArray.second, cmdList, index++);
				for (auto& table : map->tables)
					SetParameter(&table.second, cmdList, index++);
			}

		}

	}
	
}