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
		void InitConstant(_Context* context, UINT shaderRegister, UINT num32BitValues);

		void InitConstantBuffer(_Context* context, UINT shaderRegister);
		void InitConstantBuffer(_TableParameter* context, UINT shaderRegister);

		void InitArrayBuffer(_Context* context, UINT shaderRegister);
		void InitArrayBuffer(_TableParameter* context, UINT shaderRegister);

		void InitRWArrayBuffer(_Context* context, UINT shaderRegister);
		void InitRWArrayBuffer(_TableParameter* context, UINT shaderRegister);

		void InitParameterTable(_Context* context, std::vector<_TableParameter> parameters);

	}

	struct _TableParameter {

		_TableParameter& ConstantBuffer(UINT shaderRegister);
		_TableParameter& ArrayBuffer(UINT shaderRegister);
		_TableParameter& RWArrayBuffer(UINT shaderRegister);

		operator InputParameter::_TableParameter() noexcept;

	protected:

		InputParameter::_TableParameter context = {};

	};

	struct _InputParameter {

		_InputParameter& Constant(UINT shaderRegister, UINT num32BitValues);

		_InputParameter& ConstantBuffer(UINT shaderRegister);
		_InputParameter& ArrayBuffer(UINT shaderRegister);
		_InputParameter& RWArrayBuffer(UINT shaderRegister);

		_InputParameter& ParameterTable(std::vector<InputParameter::_TableParameter> parameters);

		operator D3D12_ROOT_PARAMETER() noexcept;

	protected:

		InputParameter::_Context context = {};

	};

}