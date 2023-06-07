module;

#include "GraphicsHeader.h"

export module TR.Graphics.InputParameter;

export import TR.Graphics.Device;
export import <vector>;

export namespace TR::Graphics {

	namespace InputParameter {

		struct _Context {
			D3D12_ROOT_PARAMETER parameter = {};
			std::vector<D3D12_DESCRIPTOR_RANGE> ranges = {};
		};
		struct _TableParameter {
			D3D12_DESCRIPTOR_RANGE_TYPE rangeType;
			UINT shaderRegister;
		};

		// Maybe add register space

		// Not supported
		void InitConstant(_Context* inputParameter, UINT shaderRegister, UINT num32BitValues);

		void InitConstantBuffer(_Context* inputParameter, UINT shaderRegister);
		void InitConstantBuffer(_TableParameter* tableParameter, UINT shaderRegister);

		void InitArrayBuffer(_Context* inputParameter, UINT shaderRegister);
		void InitArrayBuffer(_TableParameter* tableParameter, UINT shaderRegister);

		void InitRWArrayBuffer(_Context* inputParameter, UINT shaderRegister);
		void InitRWArrayBuffer(_TableParameter* tableParameter, UINT shaderRegister);

		void InitParameterTable(_Context* inputParameter, std::vector<_TableParameter> parameters);

	}

	struct _TableParameter {

		InputParameter::_TableParameter tableParameter = {};

		_TableParameter& ConstantBuffer(UINT shaderRegister);
		_TableParameter& ArrayBuffer(UINT shaderRegister);
		_TableParameter& RWArrayBuffer(UINT shaderRegister);

		operator InputParameter::_TableParameter() noexcept;

	};

	struct _InputParameter {

		InputParameter::_Context inputParameter = {};

		_InputParameter& Constant(UINT shaderRegister, UINT num32BitValues);

		_InputParameter& ConstantBuffer(UINT shaderRegister);
		_InputParameter& ArrayBuffer(UINT shaderRegister);
		_InputParameter& RWArrayBuffer(UINT shaderRegister);

		_InputParameter& ParameterTable(std::vector<InputParameter::_TableParameter> parameters);

		operator D3D12_ROOT_PARAMETER() noexcept;

	};

}