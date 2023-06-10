module;

#include "GraphicsHeader.h"

export module TR.Graphics.Renderer;

export import TR.Graphics.Device;
export import TR.Graphics.InputParameter;
export import TR.Graphics.VertexLayout;
export import TR.Graphics.Shader;

export namespace TR::Graphics {

	namespace Renderer {

		struct E_FailedPipelineStateCreation : public _D3D12Exception {
			E_FailedPipelineStateCreation(HRESULT returnValue) : _D3D12Exception("Failed to create pipeline state.", returnValue) {}
		};

		struct _Context {
			VertexLayout::_Context vertexLayout = {};
			Shader::_Set shaderSet = {};
			D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			DXGI_FORMAT renderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			ComPtr<ID3D12PipelineState> pipelineState = {};
		};

		void Init(_Context* renderer, ID3D12RootSignature* rootSignature);

		void Render(_Context* renderer, ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW vertexBuffer, InputParameter::Map::_Context* inputMap, D3D12_VIEWPORT viewPort, RECT scissorRect);

	}

	namespace Renderer {

		void Init(_Context* renderer, InputParameter::Map::_Context* inputMap);

	}

	struct _Renderer {

		Renderer::_Context renderer = {};
		InputParameter::Map::_Context inputMap = {};

		void Init();
		// Add Init that takes in relevant arguments too
		
		void Render(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW vertexBuffer, D3D12_VIEWPORT viewPort, RECT scissorRect);


	};

}