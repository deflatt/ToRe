module;

#include "GraphicsHeader.h"

module TR.Graphics.InputParameter;

import TR.Graphics.Device;

namespace TR::Graphics {

	namespace InputParameter {

		void InitConstant(_Context* inputParameter, UINT shaderRegister, UINT num32BitValues)
		{
			auto& [parameter, ranges] = *inputParameter;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
			parameter.Constants.Num32BitValues = num32BitValues;
		}

		void InitConstantBuffer(_Context* inputParameter, UINT shaderRegister)
		{
			auto& [parameter, ranges] = *inputParameter;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
		}

		void InitConstantBuffer(_TableParameter* tableParameter, UINT shaderRegister)
		{
			tableParameter->rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			tableParameter->shaderRegister = shaderRegister;
		}

		void InitArrayBuffer(_Context* inputParameter, UINT shaderRegister)
		{
			auto& [parameter, ranges] = *inputParameter;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
		}

		void InitArrayBuffer(_TableParameter* tableParameter, UINT shaderRegister)
		{
			tableParameter->rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			tableParameter->shaderRegister = shaderRegister;
		}

		void InitRWArrayBuffer(_Context* inputParameter, UINT shaderRegister)
		{
			auto& [parameter, ranges] = *inputParameter;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
		}

		void InitRWArrayBuffer(_TableParameter* tableParameter, UINT shaderRegister)
		{
			tableParameter->rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			tableParameter->shaderRegister = shaderRegister;
		}

		void InitParameterTable(_Context* inputParameter, std::vector<_TableParameter> parameters)
		{
			auto& [parameter, ranges] = *inputParameter;
			
			// Not perfect distribution but whatever
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			for (UINT i = 0; i < parameters.size(); i++) {
				if (i == 0 || !(parameters[i].rangeType == parameters[i - 1].rangeType
					&& parameters[i].shaderRegister == (parameters[i - 1].shaderRegister + 1))) {

					D3D12_DESCRIPTOR_RANGE range = {};
					range.BaseShaderRegister = parameters[i].shaderRegister;
					range.NumDescriptors = 1;
					range.OffsetInDescriptorsFromTableStart = 0;
					range.RangeType = parameters[i].rangeType;

					ranges.push_back(range);
					parameter.DescriptorTable.NumDescriptorRanges++;
				}
				else {
					ranges.back().NumDescriptors++;
				}
			}
			
		}

	}

	_TableParameter& _TableParameter::ConstantBuffer(UINT shaderRegister)
	{
		InputParameter::InitConstantBuffer(&tableParameter, shaderRegister);
		return *this;
	}

	_TableParameter& _TableParameter::ArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitArrayBuffer(&tableParameter, shaderRegister);
		return *this;
	}

	_TableParameter& _TableParameter::RWArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitRWArrayBuffer(&tableParameter, shaderRegister);
		return *this;
	}

	_TableParameter::operator InputParameter::_TableParameter() noexcept
	{
		return tableParameter;
	}

	_InputParameter& _InputParameter::Constant(UINT shaderRegister, UINT num32BitValues)
	{
		InputParameter::InitConstant(&inputParameter, shaderRegister, num32BitValues);
		return *this;
	}

	_InputParameter& _InputParameter::ConstantBuffer(UINT shaderRegister)
	{
		InputParameter::InitConstantBuffer(&inputParameter, shaderRegister);
		return *this;
	}

	_InputParameter& _InputParameter::ArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitArrayBuffer(&inputParameter, shaderRegister);
		return *this;
	}

	_InputParameter& _InputParameter::RWArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitRWArrayBuffer(&inputParameter, shaderRegister);
		return *this;
	}

	_InputParameter& _InputParameter::ParameterTable(std::vector<InputParameter::_TableParameter> parameters)
	{
		InputParameter::InitParameterTable(&inputParameter, parameters);
		return *this;
	}

	_InputParameter::operator D3D12_ROOT_PARAMETER() noexcept
	{
		return inputParameter.parameter;
	}

}