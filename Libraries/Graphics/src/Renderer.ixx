module;

#include "GraphicsHeader.h"

export module TR.Graphics.Renderer;

export import TR.Graphics.Device;
export import TR.Graphics.VertexLayout;
export import TR.Graphics.Shader;

export namespace TR::Graphics {

	namespace Renderer {

		struct E_FailedRootSignatureSerialization : public _D3D12Exception {
			E_FailedRootSignatureSerialization(HRESULT returnValue) : _D3D12Exception("Failed to serialize root signature.", returnValue) {}
		};
		struct E_FailedRootSignatureCreation : public _D3D12Exception {
			E_FailedRootSignatureCreation(HRESULT returnValue) : _D3D12Exception("Failed to create root signature.", returnValue) {}
		};
		struct E_FailedPipelineStateCreation : public _D3D12Exception {
			E_FailedPipelineStateCreation(HRESULT returnValue) : _D3D12Exception("Failed to create pipeline state.", returnValue) {}
		};

		struct _Context {
			std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};
			std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = {};
			VertexLayout::_Context vertexLayout = {};
			Shader::_Set shaderSet = {};
			D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			DXGI_FORMAT renderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			ComPtr<ID3D12PipelineState> pipelineState = {};
			ComPtr<ID3D12RootSignature> rootSignature = {};
		};

		void Init(_Context* context);

	}

}