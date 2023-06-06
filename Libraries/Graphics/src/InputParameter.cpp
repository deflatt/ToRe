module;

#include "GraphicsHeader.h"

module TR.Graphics.InputParameter;

import TR.Graphics.Device;

namespace TR::Graphics {

	namespace InputParameter {

		void InitConstant(_Context* context, UINT shaderRegister, UINT num32BitValues)
		{
			auto& [parameter, ranges] = *context;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
			parameter.Constants.Num32BitValues = num32BitValues;
		}

		void InitConstantBuffer(_Context* context, UINT shaderRegister)
		{
			auto& [parameter, ranges] = *context;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
		}

		void InitConstantBuffer(_TableParameter* context, UINT shaderRegister)
		{
			context->rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			context->shaderRegister = shaderRegister;
		}

		void InitArrayBuffer(_Context* context, UINT shaderRegister)
		{
			auto& [parameter, ranges] = *context;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
		}

		void InitArrayBuffer(_TableParameter* context, UINT shaderRegister)
		{
			context->rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			context->shaderRegister = shaderRegister;
		}

		void InitRWArrayBuffer(_Context* context, UINT shaderRegister)
		{
			auto& [parameter, ranges] = *context;
			parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
			parameter.Constants.ShaderRegister = shaderRegister;
			parameter.Constants.RegisterSpace = 0;
		}

		void InitRWArrayBuffer(_TableParameter* context, UINT shaderRegister)
		{
			context->rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
			context->shaderRegister = shaderRegister;
		}

		void InitParameterTable(_Context* context, std::vector<_TableParameter> parameters)
		{
			auto& [parameter, ranges] = *context;
			
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
		InputParameter::InitConstantBuffer(&context, shaderRegister);
		return *this;
	}

	_TableParameter& _TableParameter::ArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitArrayBuffer(&context, shaderRegister);
		return *this;
	}

	_TableParameter& _TableParameter::RWArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitRWArrayBuffer(&context, shaderRegister);
		return *this;
	}

	_TableParameter::operator InputParameter::_TableParameter() noexcept
	{
		return context;
	}

	_InputParameter& _InputParameter::Constant(UINT shaderRegister, UINT num32BitValues)
	{
		InputParameter::InitConstant(&context, shaderRegister, num32BitValues);
		return *this;
	}

	_InputParameter& _InputParameter::ConstantBuffer(UINT shaderRegister)
	{
		InputParameter::InitConstantBuffer(&context, shaderRegister);
		return *this;
	}

	_InputParameter& _InputParameter::ArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitArrayBuffer(&context, shaderRegister);
		return *this;
	}

	_InputParameter& _InputParameter::RWArrayBuffer(UINT shaderRegister)
	{
		InputParameter::InitRWArrayBuffer(&context, shaderRegister);
		return *this;
	}

	_InputParameter& _InputParameter::ParameterTable(std::vector<InputParameter::_TableParameter> parameters)
	{
		InputParameter::InitParameterTable(&context, parameters);
		return *this;
	}

	_InputParameter::operator D3D12_ROOT_PARAMETER() noexcept
	{
		return context.parameter;
	}

}